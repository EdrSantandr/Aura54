// EdrSantandr Dev

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraMainGoal.generated.h"

class AAuraLifePoint;
class USphereComponent;

UCLASS()
class AURA_API AAuraMainGoal : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraMainGoal();

	UFUNCTION(BlueprintImplementableEvent, Category="ReachGoal")
	void SetDarkLightning();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category="ReachGoal")
	TArray<AActor*> MapPoints;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="ReachGoal")
	TObjectPtr<UStaticMeshComponent> MainGoalMesh;

	UPROPERTY(VisibleAnywhere, Category="ReachGoal")
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(EditAnywhere, Category="ReachGoal")
	TArray<TSubclassOf<AAuraLifePoint>> LifePointsClasses;

	UPROPERTY(EditDefaultsOnly, Category="ReachGoal")
	float MinLifePointDistance = 150.f;

	UPROPERTY(EditDefaultsOnly, Category="ReachGoal")
	float MaxLifePointDistance = 450.f;
	
	void SpawnLifePoints(int32 NumberOfLives);

	void EliminateLifePoints(int32 InLifePoints);
	
	TSubclassOf<AAuraLifePoint> GetRandomLifePointClass();
	
	TArray<TSoftObjectPtr<AAuraLifePoint>> LifePoints;
	
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
