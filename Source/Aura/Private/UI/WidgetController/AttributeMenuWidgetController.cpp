// EdrSantander Dev


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Player/AuraPlayerState.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	if (GetAuraAS()!=nullptr)
	{
		check(AttributeInfo);

		for (auto& Pair : GetAuraAS()->TagsToAttributes)
		{
			BroadcastAttributeInfo(Pair.Key, Pair.Value());
		}

		if (GetAuraPS()!=nullptr)
		{
			AttributePointsChangedDelegate.Broadcast(GetAuraPS()->GetAttributePoints());
		}
	}
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	if (GetAuraAS()!=nullptr)
	{
		for(auto& Pair : GetAuraAS()->TagsToAttributes)
		{
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
				{
					BroadcastAttributeInfo(Pair.Key, Pair.Value());
				}
			);
		}
	}
	
	if (GetAuraPS()!=nullptr)
	{
		GetAuraPS()->OnAttributePointsChangedDelegate.AddLambda(
			[this](int32 Points)
			{
				AttributePointsChangedDelegate.Broadcast(Points);
			}
		);
	}
}

void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	if (GetAuraASC()!=nullptr)
	{
		GetAuraASC()->UpgradeAttribute(AttributeTag);
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoByTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoSignature.Broadcast(Info);
}
