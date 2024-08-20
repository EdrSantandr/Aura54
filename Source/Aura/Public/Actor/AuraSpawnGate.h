// EdrSantandr Dev

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "GameFramework/Actor.h"
#include "AuraSpawnGate.generated.h"

class AAuraMainGoal;
class AAuraEnemy;
class AAuraCharacterBase;

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
	
	UFUNCTION()
	void SpawnEnemy();

	UFUNCTION()
	FTransform GenerateRandomTransform();
	
private:
	FTimerDelegate TimerDelegate;
	FTimerHandle TimerHandle;

	TArray<FVector> SpawnLocations;
	
	int32 EnemiesSpawned = 0;
};
