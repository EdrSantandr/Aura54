// EdrSantandr Dev

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AllyInterface.generated.h"

enum class ECharacterClass : uint16;

UINTERFACE(MinimalAPI)
class UAllyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IAllyInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	ECharacterClass GetCharacterClass();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetCombatTargetAlly(AActor* InCombatTarget);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	AActor* GetCombatTargetAlly() const;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetLevel(int32 NewLevel);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool GetIsGate();
};
