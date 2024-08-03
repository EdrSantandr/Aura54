// EdrSantandr Dev

#pragma once

#include "CoreMinimal.h"
#include "Actor/AuraProjectile.h"
#include "AuraFireball.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraFireball : public AAuraProjectile
{
	GENERATED_BODY()

public:
	
protected:
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void BeginPlay() override;
};
