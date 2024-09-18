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
	void SpawnProjectiles(const FVector& ProjectileTargetLocation, bool bOverridePitch, float PitchOverride, AActor* HomingTarget, const FVector& SocketLocation);

	UFUNCTION(BlueprintCallable)
	void SpawnSpreadProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride, AActor* HomingTarget);

	UFUNCTION(BlueprintCallable)
	void SpawnSpreadProjectilesWithOrigin(const FVector& InProjectileTargetLocation, const FVector& InOriginLocation, bool bOverridePitch, float PitchOverride, AActor* HomingTarget);

protected:	
	UPROPERTY(EditDefaultsOnly, Category="Firebolt")
	float ProjectileSpread = 90.f;

	UPROPERTY(EditDefaultsOnly, Category="Firebolt")
	int32 MaxNumProjectiles = 5;

	UPROPERTY(EditDefaultsOnly, Category="Firebolt")
	bool bLaunchHomingProjectiles = true;
};
