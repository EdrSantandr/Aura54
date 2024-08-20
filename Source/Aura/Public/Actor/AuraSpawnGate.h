// EdrSantandr Dev

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "GameFramework/Actor.h"
#include "AuraSpawnGate.generated.h"

class AAuraMainGoal;
class AAuraEnemy;
class AAuraCharacterBase;

struct FSortVectorByDistance
{
	explicit FSortVectorByDistance(const FVector& InSourceLocation)
		: SourceLocation(InSourceLocation) {}
	
	FVector SourceLocation = FVector::Zero();

	bool operator()(const AActor* A, const AActor* B) const
	{
		float DistanceA = FVector::DistSquared(SourceLocation, A->GetActorLocation());
		float DistanceB = FVector::DistSquared(SourceLocation, B->GetActorLocation());

		return DistanceA < DistanceB;
	}
};

UCLASS()
class AURA_API AAuraSpawnGate : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraSpawnGate();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="SpawnGate")
	TObjectPtr<AActor> MainGoal;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category="SpawnGate")
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SpawnGate")
	TSubclassOf<AAuraEnemy> SpawnClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SpawnGate")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;
	
	UPROPERTY(EditDefaultsOnly, Category="SpawnGate")
	int32 NumberOfEnemiesToSpawn = 5;

	UPROPERTY(EditDefaultsOnly, Category="SpawnGate")
	float SpawnInterval = 2.f;

	UPROPERTY(EditDefaultsOnly, Category="SpawnGate")
	float DistanceFromGate = 50.f;

	UPROPERTY(EditDefaultsOnly, Category="SpawnGate")
	float AngleSpread = 90.f;

	UPROPERTY(EditDefaultsOnly, Category="SpawnGate")
	int32 SpawnApertures = 4;

	UPROPERTY(EditDefaultsOnly, Category="SpawnGate")
	float YPathDimension = 1000.f;
	
	UFUNCTION()
	void SpawnEnemy();

	UFUNCTION()
	FTransform GenerateRandomTransform();

	UFUNCTION()
	TArray<FVector> GetEnemyPath();
	
private:
	
	TArray<TArray<FVector>> PathsByPoint;

	UFUNCTION()
	void CreatePathsFromGate(const int32 NumPaths, const FVector& InOriginalPoint, const FVector& InFinalPoint);
	
	static TArray<FVector> CreateSinglePath(const TArray<AActor*>& InActors, const FVector& InOriginalPoint, const FVector& InFinalPoint);
	
	FTimerDelegate TimerDelegate;
	FTimerHandle TimerHandle;

	TArray<FVector> SpawnLocations;
	
	int32 EnemiesSpawned = 0;
};
