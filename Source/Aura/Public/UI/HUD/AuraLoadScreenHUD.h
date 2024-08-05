// EdrSantandr Dev

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraLoadScreenHUD.generated.h"

class UMVVM_LoadScreen;
class UAuraLoadScreenWidget;
/**
 * 
 */
UCLASS()
class AURA_API AAuraLoadScreenHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> LoadScreenWidgetClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UAuraLoadScreenWidget> LoadScreenWidget;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_LoadScreen> LoadScreenViewModelClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UMVVM_LoadScreen> LoadScreenViewModel;

protected:
	virtual void BeginPlay() override;
};
