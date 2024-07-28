// EdrSantandr Dev

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraProjectileSpell.h"
#include "AuraFirebolt.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraFirebolt : public UAuraProjectileSpell
{
	GENERATED_BODY()

public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;
};
