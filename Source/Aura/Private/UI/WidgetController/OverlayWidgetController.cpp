// EdrSantander Dev


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Player/AuraPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	if (const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet))
	{
		OnHealthChangedSignature.Broadcast(AuraAttributeSet->GetHealth());
		OnMaxHealthChangedSignature.Broadcast(AuraAttributeSet->GetMaxHealth());
		OnManaChangedSignature.Broadcast(AuraAttributeSet->GetMana());
		OnMaxManaChangedSignature.Broadcast(AuraAttributeSet->GetMaxMana());
	}
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	//Bind Callbacks to player state
	if (AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState))
	{
		AuraPlayerState->OnXPChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXpChanged);
		AuraPlayerState->OnLevelChangedDelegate.AddLambda(
			[this](int32 NewLevel)
			{
				OnPlayerLevelChangedDelegate.Broadcast(NewLevel);
			}
		);
	}
	//Bind Callbacks to Asc
	if (const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChangedSignature.Broadcast(Data.NewValue);
			}
		);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChangedSignature.Broadcast(Data.NewValue);
			}
		);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnManaChangedSignature.Broadcast(Data.NewValue);
			}
		);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxManaChangedSignature.Broadcast(Data.NewValue);
			}
		);

		if (UAuraAbilitySystemComponent* AuraAsc = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
		{
			if (AuraAsc->bStartupAbilitiesGiven)
			{
				OnInitializeStartupAbilities(AuraAsc);
			}
			else
			{
				AuraAsc->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::OnInitializeStartupAbilities);	
			}
			
			AuraAsc->EffectAssetTags.AddLambda(
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

void UOverlayWidgetController::OnInitializeStartupAbilities(UAuraAbilitySystemComponent* AuraAsc)
{
	//TODO get information about all given abilities, look out for their info and broadcast the info to all widgets
	if (!AuraAsc->bStartupAbilitiesGiven) return;

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this, AuraAsc](const FGameplayAbilitySpec& AbilitySpec)
		{
			//TODO need a weay to figure out the ability tag for a given ability spec
			FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AuraAsc->GetAbilityTagFromSpec(AbilitySpec));
			Info.InputTag = AuraAsc->GetInputTagFromSpec(AbilitySpec);
			AbilityInfoDelegate.Broadcast(Info);
		}
	);
	AuraAsc->ForEachAbility(BroadcastDelegate);
}

void UOverlayWidgetController::OnXpChanged(int32 NewXp) const
{
	if (const AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(PlayerState))
	{
		const ULevelUpInfo* LevelUpInfo = AuraPlayerState->LevelUpInfo;
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
