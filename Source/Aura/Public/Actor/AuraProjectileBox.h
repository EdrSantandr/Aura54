// EdrSantandr Dev

#pragma once

#include "CoreMinimal.h"
#include "Actor/AuraProjectile.h"
#include "AuraProjectileBox.generated.h"

class UBoxComponent;
/**
 * 
 */
UCLASS()
class AURA_API AAuraProjectileBox : public AAuraProjectile
{
	GENERATED_BODY()

public:
	AAuraProjectileBox();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UBoxComponent> BoxComponent;

	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	void DealDamage();

	UFUNCTION()
	virtual void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UPROPERTY()
	TArray<TObjectPtr<AActor>> CollidedActors;
};
