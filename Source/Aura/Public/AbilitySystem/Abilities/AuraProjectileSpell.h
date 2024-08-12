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

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AAuraProjectile> ProjectileClass;

	UFUNCTION(BlueprintCallable, Category="Projecttile")
	void SpawnProjectile(const FVector& TargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch = false, float PitchOverride = 0.f);

	UFUNCTION(BlueprintCallable, Category="Projecttile")
	void SpawnProjectileAtLocation(const FVector& TargetLocation, const FVector& SpawnLocation, bool bOverridePitch = false, float PitchOverride = 0.f);

	UPROPERTY(EditDefaultsOnly)
	int32 NumProjectiles = 5;
};
