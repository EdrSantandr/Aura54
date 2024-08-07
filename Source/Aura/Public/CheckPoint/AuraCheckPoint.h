// EdrSantandr Dev

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/SaveInterface.h"
#include "AuraCheckPoint.generated.h"

class USphereComponent;
/**
 * 
 */
UCLASS()
class AURA_API AAuraCheckPoint : public APlayerStart, public ISaveInterface
{
	GENERATED_BODY()

public:
	AAuraCheckPoint(const FObjectInitializer& ObjectInitializer);

	/*Save interface*/
	virtual bool ShouldLoadTransform_Implementation() override;
	virtual void LoadActor_Implementation() override;

	UPROPERTY(BlueprintReadOnly, SaveGame)
	bool bReached = false;
	
protected:
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void CheckPointReach(UMaterialInstanceDynamic* DynamicMaterialInstance);

	void HandleGlowEffects(); 
	
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> CheckPointMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> SphereComponent;
};
