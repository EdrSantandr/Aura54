// EdrSantandr Dev

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetWidgetSwitcherIndexSignature, int32, WidgetSwticherIndex);

/**
 * 
 */
UCLASS()
class AURA_API UMVVM_LoadSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FSetWidgetSwitcherIndexSignature SetWidgetSwitcherIndexDelegate;

	void InitializeSlot();
	
	void SetLoadSlotName(FString InLoadSlotName);

	FString GetLoadSlotName() const { return LoadSlotName; };

	UPROPERTY()
	FString PlayerName;

	UPROPERTY()
	FString LoadSlot;

	UPROPERTY()
	FString SlotIndex;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess="true"))
	FString LoadSlotName;
};
