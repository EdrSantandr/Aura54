// EdrSantander Dev


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Game/AuraGameModeBase.h"
#include "Player/AuraPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	OnHealthChangedSignature.Broadcast(GetAuraAS()->GetHealth());
	OnMaxHealthChangedSignature.Broadcast(GetAuraAS()->GetMaxHealth());
	OnManaChangedSignature.Broadcast(GetAuraAS()->GetMana());
	OnMaxManaChangedSignature.Broadcast(GetAuraAS()->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	//Bind Callbacks to GameMode
	if (GetAuraGM()!=nullptr)
	{
		GetAuraGM()->OnGateDestroyedDelegate.AddLambda(
			[this](int32 CurrentGates, int32 TotalGates )
			{
				OnGateDestroyedChangedDelegate.Broadcast(CurrentGates, TotalGates);
			}
		);
		GetAuraGM()->OnEnemyKilledDelegate.AddLambda(
			[this](int32 EnemiesKilled )
			{
				OnEnemyKilledChangedDelegate.Broadcast(EnemiesKilled);
			}
		);
		GetAuraGM()->OnPlayerLivesDelegate.AddLambda(
			[this](int32 PlayerLives )
			{
				OnPlayerLivesUpdateDelegate.Broadcast(PlayerLives);
			}
		);
		GetAuraGM()->OnAbilityPlayerMessageSignature.AddUObject(this, &UOverlayWidgetController::HandlePlayerMessage);
	}
	
	//Bind Callbacks to player state
	if (GetAuraPS()!=nullptr)
	{
		GetAuraPS()->OnXPChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXpChanged);
		GetAuraPS()->OnLevelChangedDelegate.AddLambda(
			[this](int32 NewLevel, bool bLevelUp)
			{
				OnPlayerLevelChangedDelegate.Broadcast(NewLevel,bLevelUp);
			}
		);
	}
	//Bind Callbacks to Asc
	if (GetAuraAS()!=nullptr)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAS()->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChangedSignature.Broadcast(Data.NewValue);
			}
		);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAS()->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChangedSignature.Broadcast(Data.NewValue);
			}
		);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAS()->GetManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnManaChangedSignature.Broadcast(Data.NewValue);
			}
		);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAS()->GetMaxManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxManaChangedSignature.Broadcast(Data.NewValue);
			}
		);

		if (GetAuraASC()->bStartupAbilitiesGiven)
		{
			BroadcastAbilityInfo();
		}
		else
		{
			GetAuraASC()->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::BroadcastAbilityInfo);	
		}
			
		GetAuraASC()->EffectAssetTags.AddLambda(
			[this](const FGameplayTagContainer& AssetTags)
			{
				for(const FGameplayTag& Tag : AssetTags)
				{
					FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
					if (Tag.MatchesTag(MessageTag))
					{
						const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
						MessageWidgetRowSignature.Broadcast(*Row);	
					}
				}
			}
		);
		
		GetAuraASC()->AbilityEquippedDelegate.AddUObject(this, &UOverlayWidgetController::OnAbilityEquipped);
	}
}

void UOverlayWidgetController::HandlePlayerMessage(const FString& InMessage) const
{
	OnMessagePlayerSignature.Broadcast(InMessage);
}

void UOverlayWidgetController::OnXpChanged(int32 NewXp)
{
	if (GetAuraPS()!=nullptr)
	{
		const ULevelUpInfo* LevelUpInfo = GetAuraPS()->LevelUpInfo;
		checkf(LevelUpInfo, TEXT("Unable to fin levelupinfo please check Auraplayerstate BP"));

		const int32 Level = LevelUpInfo->FindLevelForExp(NewXp);
		const int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num();
		if (Level <= MaxLevel && Level>0)
		{
			const int32 LevelUpRequirement = LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
			const int32 PreviousLevelUpRequirement = LevelUpInfo->LevelUpInformation[Level-1].LevelUpRequirement;

			const int32 DeltaLevelRequirement = LevelUpRequirement - PreviousLevelUpRequirement;
			const int32 XpForThisLevel = NewXp - PreviousLevelUpRequirement;

			const float XpBarPercent = static_cast<float>(XpForThisLevel)/static_cast<float>(DeltaLevelRequirement);

			OnXpPercentChangedDelegate.Broadcast(XpBarPercent);
		}
	}
}

void UOverlayWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const FGameplayTag& SlotInputTag, const FGameplayTag& PrevSlotInputTag) const
{
	const FAuraGameplayTags AuraGameplayTags = FAuraGameplayTags::Get(); 

	//Clear the old slot
	FAuraAbilityInfo LastSlotInfo;
	LastSlotInfo.StatusTag = AuraGameplayTags.Abilities_Status_Unlocked;
	LastSlotInfo.InputTag = PrevSlotInputTag;
	LastSlotInfo.AbilityTag = AuraGameplayTags.Abilities_None;
	//Broadcast empty info if PreviousSlot is a valid slot. Only if equipping an already-equipped spell
	AbilityInfoDelegate.Broadcast(LastSlotInfo);

	//Fill the new slot
	FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	Info.StatusTag = StatusTag;
	Info.InputTag = SlotInputTag;
	AbilityInfoDelegate.Broadcast(Info);
}
