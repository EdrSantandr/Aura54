// EdrSantander Dev

#pragma once

#include "CoreMinimal.h"
#include "AuraAbilityTypes.h"
#include "GameFramework/Actor.h"
#include "AuraProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UNiagaraSystem;

UCLASS()
class AURA_API AAuraProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraProjectile();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn = true))
	FDamageEffectParams DamageEffectParams;

	UPROPERTY()
	TObjectPtr<USceneComponent> HomingTargetSceneComponent;

	UFUNCTION(BlueprintImplementableEvent)
	void OnHitProjectile(AActor* InOtherActor);

	UFUNCTION()
	void BindTargetDestroy(AActor* HomingTarget);
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	virtual void OnHit();

	virtual void Destroyed() override;

	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> SphereComponent;

	bool bHit = false;

	UPROPERTY(EditDefaultsOnly, Category="CustomCollision")
	bool bBindRootOnOverlap = true;
	
	UPROPERTY()
	TObjectPtr<UAudioComponent> LoopingSoundComponent;

	UFUNCTION(BlueprintCallable, Category="CustomCollision")
	void AreaRadialDamage(bool bOverRideRadius = false, float RadiusOverwritten = 0.f);
	
private:

	UFUNCTION()
	void OnTargetDie(AActor* DeadActor);
	
	UPROPERTY(EditDefaultsOnly)
	float LifeSpan = 15.f;	
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> LoopingSound;
};
