// EdrSantandr Dev

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraFirebolt.h"
#include "AuraWindWave.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraWindWave : public UAuraFirebolt
{
	GENERATED_BODY()

public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;
};
