// EdrSantandr Dev

#pragma once

#include "CoreMinimal.h"
#include "CheckPoint/AuraCheckPoint.h"
#include "AuraMapEntrance.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraMapEntrance : public AAuraCheckPoint
{
	GENERATED_BODY()

public:
	AAuraMapEntrance(const FObjectInitializer& ObjectInitializer);
	
	/* HighLight Interface*/
	virtual void HighlightActor_Implementation() override;

	/*Save Interface*/
	virtual void LoadActor_Implementation() override;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UWorld> DestinationMap;

	UPROPERTY(EditAnywhere)
	FName DestinationPlayerStartTag;

protected:
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
