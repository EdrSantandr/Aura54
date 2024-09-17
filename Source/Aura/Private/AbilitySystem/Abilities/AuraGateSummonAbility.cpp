// EdrSantandr Dev


#include "AbilitySystem/Abilities/AuraGateSummonAbility.h"

#include "Character/AuraAlly.h"
#include "Character/AuraEnemy.h"

void UAuraGateSummonAbility::SpawnEnemy(const int32 InEnemiesSpawned, const int32 InNumberOfEnemiesToSpawn, TArray<FVector> InSpawnLocations)
{
	if (InEnemiesSpawned < InNumberOfEnemiesToSpawn)
	{
		if (AAuraAlly*AuraAlly = Cast<AAuraAlly>(GetActorInfo().AvatarActor))
		{
			const TArray<TArray<FVector>> PathsByPoint = AuraAlly->GetPathPoints();
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			AAuraEnemy* Enemy = GetWorld()->SpawnActorDeferred<AAuraEnemy>(GetSpawnClass(), GetActorInfo().AvatarActor->GetActorTransform());
			Enemy->SetLevel(GetAbilityLevel());
			Enemy->SetPath(GetEnemyPath(PathsByPoint));
			Enemy->FinishSpawning(GenerateRandomTransform(InSpawnLocations));
			Enemy->SpawnDefaultController();
			AuraAlly->IncreaseEnemiesSpawned(1);
		}
	}
}

TSubclassOf<AAuraEnemy> UAuraGateSummonAbility::GetSpawnClass()
{
	const int32 RandomInt = FMath::RandRange(0, SpawnClasses.Num()-1);
	return SpawnClasses[RandomInt];
}

FTransform UAuraGateSummonAbility::GenerateRandomTransform(TArray<FVector> InSpawnLocations) const
{
	FTransform Result;
	const int32 Index = FMath::RandRange(0, InSpawnLocations.Num()-1);
	FVector Location = GetActorInfo().AvatarActor->GetActorLocation() + InSpawnLocations[Index]*DistanceFromGate;
	Location +=FVector(0.f,0.f,SpawnHeightAdjustment);
	Result.SetLocation(Location);
	Result.SetRotation(InSpawnLocations[Index].ToOrientationQuat());
	return Result;
}

TArray<FVector> UAuraGateSummonAbility::GetEnemyPath(const TArray<TArray<FVector>>& InPathsByPoint)
{
	const int32 RandPathIndex = FMath::RandRange(0, InPathsByPoint.Num()-1);
	return InPathsByPoint[RandPathIndex];
}
