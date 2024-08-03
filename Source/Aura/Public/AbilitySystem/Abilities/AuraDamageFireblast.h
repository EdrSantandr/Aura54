// EdrSantandr Dev

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "AuraDamageFireblast.generated.h"

class AAuraFireball;
/**
 * 
 */
UCLASS()
class AURA_API UAuraDamageFireblast : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;

	UFUNCTION(BlueprintCallable)
	TArray<AAuraFireball*> SpawnFireBalls(); 

protected:
	UPROPERTY(EditDefaultsOnly, Category="Fireblast")
	int32 NumFireBalls = 12;

private:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AAuraFireball> FireballClass;
};
