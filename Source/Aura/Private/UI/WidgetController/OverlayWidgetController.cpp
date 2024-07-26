// EdrSantander Dev


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
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
	//Bind Callbacks to player state
	if (GetAuraPS()!=nullptr)
	{
		GetAuraPS()->OnXPChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXpChanged);
		GetAuraPS()->OnLevelChangedDelegate.AddLambda(
			[this](int32 NewLevel)
			{
				OnPlayerLevelChangedDelegate.Broadcast(NewLevel);
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

		if (GetAuraASC()!=nullptr)
		{
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
		} 
	}
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
