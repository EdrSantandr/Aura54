// EdrSantander Dev

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "OverlayWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnValueChangedSignature, float, NewValue);
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnValueChangedSignature OnHealthChangedSignature;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnValueChangedSignature OnMaxHealthChangedSignature;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnValueChangedSignature OnManaChangedSignature;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnValueChangedSignature OnMaxManaChangedSignature;

protected:
	void HealthChange(const FOnAttributeChangeData& Data) const;
	void MaxHealthChange(const FOnAttributeChangeData& Data) const;
	void ManaChange(const FOnAttributeChangeData& Data) const;
	void MaxManaChange(const FOnAttributeChangeData& Data) const;
};
