// EdrSantandr Dev

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraMainGoal.generated.h"

class USphereComponent;

UCLASS()
class AURA_API AAuraMainGoal : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraMainGoal();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category="ReachGoal")
	TArray<AActor*> MapPoints;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="ReachGoal")
	TObjectPtr<UStaticMeshComponent> MainGoalMesh;

	UPROPERTY(VisibleAnywhere, Category="ReachGoal")
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(EditAnywhere, Category="ReachGoal")
	TArray<AActor*> LifePoints;

	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
