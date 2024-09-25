// EdrSantandr Dev

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraFirebolt.h"
#include "AuraInvoke.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraInvoke : public UAuraFirebolt
{
	GENERATED_BODY()
public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;
};
