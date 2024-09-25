// EdrSantandr Dev

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraFirebolt.h"
#include "AuraMeteor.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraMeteor : public UAuraFirebolt
{
	GENERATED_BODY()
public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;
};
