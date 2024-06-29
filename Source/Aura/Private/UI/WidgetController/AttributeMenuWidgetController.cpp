// EdrSantander Dev


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "AuraGameplayTags.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	const UAuraAttributeSet* AuraAS = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(AttributeInfo);

	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoByTag(FAuraGameplayTags::Get().Attributes_Primary_Strength);
	Info.AttributeValue = AuraAS->GetStrength();
	AttributeInfoSignature.Broadcast(Info);
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
}
