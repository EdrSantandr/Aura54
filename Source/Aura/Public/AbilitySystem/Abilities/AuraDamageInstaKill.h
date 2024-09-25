// EdrSantandr Dev

#pragma once

#include "CoreMinimal.h"
#include "AuraFirebolt.h"
#include "AuraDamageInstaKill.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraDamageInstaKill : public UAuraFirebolt
{
	GENERATED_BODY()
public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;
};
