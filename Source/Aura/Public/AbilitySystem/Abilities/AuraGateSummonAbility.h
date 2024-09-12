// EdrSantandr Dev

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AuraGateSummonAbility.generated.h"

class AAuraEnemy;
/**
 * 
 */
UCLASS()
class AURA_API UAuraGateSummonAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SpawnGate")
	TArray<TSubclassOf<AAuraEnemy>> SpawnClasses;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SpawnGate")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	UPROPERTY(EditDefaultsOnly, Category="SpawnGate")
	float DistanceFromGate = 80.f;

	UPROPERTY(EditDefaultsOnly, Category="SpawnGate")
	float SpawnHeightAdjustment = -30.f;
	
	UFUNCTION(BlueprintCallable, Category="SpawnGate")
	void SpawnEnemy(const int32 InEnemiesSpawned, const int32 InNumberOfEnemiesToSpawn, TArray<FVector> InSpawnLocations);

private:

	UFUNCTION()
	TSubclassOf<AAuraEnemy> GetSpawnClass();
	
	UFUNCTION()
	FTransform GenerateRandomTransform(TArray<FVector> SpawnLocations) const;
	
	static TArray<FVector> GetEnemyPath(const TArray<TArray<FVector>>& InPathsByPoint);
};
