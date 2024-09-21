// EdrSantander Dev

#pragma once

#include "CoreMinimal.h"
#include "AuraDamageGameplayAbility.h"
#include "AuraProjectileSpell.generated.h"

class AAuraProjectile;
class UGameplayEffect;
struct FGameplayTag;
/**
 * 
 */
UCLASS()
class AURA_API UAuraProjectileSpell : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category="Validate")
	bool CheckAbilityRange(const FVector InTargetLocation);

	UFUNCTION(BlueprintCallable, Category="Projecttile")
	static FVector GetSpawnProjectileLocationPoint(const FVector InActorLocation,const FVector InEnemyLocation, const float InDistance = 50.f);
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AAuraProjectile> ProjectileClass;

	UFUNCTION(BlueprintCallable, Category="Projecttile")
	void SpawnProjectile(const FVector& TargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch = false, float PitchOverride = 0.f);

	UFUNCTION(BlueprintCallable, Category="Projecttile")
	void SpawnProjectileAtLocation(AActor* HomingTarget, const FVector& SpawnLocation, bool bOverridePitch = false, float PitchOverride = 0.f);

	UFUNCTION(BlueprintCallable, Category="Projecttile")
	AAuraProjectile* SpawnProjectileNoTarget(const FVector& SpawnLocation, const FVector PositionAdjustment = FVector::ZeroVector);

	UFUNCTION(BlueprintCallable, Category="Projecttile")
	void UpdateProjectileTarget(AAuraProjectile* InProjectile, FVector InProjectileTargetLocation);
	
	UPROPERTY(EditDefaultsOnly)
	int32 NumProjectiles = 5;

	UPROPERTY(EditDefaultsOnly, Category="Projecttile")
	float ProjectileMaxRange = 500.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Firebolt")
	float HomingAccelerationMin = 1600.f;

	UPROPERTY(EditDefaultsOnly, Category="Firebolt")
	float HomingAccelerationMax = 3200.f;
};
