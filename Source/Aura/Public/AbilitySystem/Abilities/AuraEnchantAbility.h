// EdrSantandr Dev

#pragma once

#include "CoreMinimal.h"
#include "AuraDamageBeamSpell.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AuraEnchantAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraEnchantAbility : public UAuraDamageBeamSpell
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enchanting")
	ECharacterClass NewCharacterClass;

	UFUNCTION(BlueprintCallable)
	void EnchantAlly(AActor* InAlly);
};
