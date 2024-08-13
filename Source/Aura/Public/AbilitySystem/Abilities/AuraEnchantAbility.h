// EdrSantandr Dev

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AuraEnchantAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraEnchantAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enchanting")
	ECharacterClass CharacterClass;
};
