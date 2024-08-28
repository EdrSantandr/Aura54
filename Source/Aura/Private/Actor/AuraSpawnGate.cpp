// EdrSantandr Dev


#include "Actor/AuraSpawnGate.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Character/AuraEnemy.h"

AAuraSpawnGate::AAuraSpawnGate()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	MeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SetRootComponent(MeshComponent);
	PrimaryActorTick.bCanEverTick = false;
}

void AAuraSpawnGate::CreatePathsFromGate(const int32 NumPaths, const FVector& InOriginalPoint, const FVector& InFinalPoint)
{
	TArray<AActor*> ActorsFound;
	TArray<AActor*> ActorsToIgnore; //todo: add actors to ignore
	const float XPathDimension = (InFinalPoint - InOriginalPoint).Length()*0.5f - 25.f; //This 5.f is just to don't consider Initial and FinalPoints
	UAuraAbilitySystemLibrary::GetPathPointsInsideBox(GetWorld(), ActorsFound, ActorsToIgnore, InOriginalPoint, InFinalPoint, XPathDimension, YPathDimension);
	Algo::Sort(ActorsFound, FSortVectorByDistance(InOriginalPoint));
	for (int32 i=0;i<NumPaths; i++)
	{
		TArray<FVector> PathToGoal = CreateSinglePath(ActorsFound, InOriginalPoint, InFinalPoint);
		PathsByPoint.Add(PathToGoal);
	}
}

void AAuraSpawnGate::BeginPlay()
{
	Super::BeginPlay();
	//Create paths
	if (IsValid(MainGoal))
	{
		CreatePathsFromGate(3, GetActorLocation(), MainGoal->GetActorLocation());
		FVector Direction = MainGoal->GetActorLocation() - GetActorLocation();
		Direction.Normalize();
		SpawnLocations = UAuraAbilitySystemLibrary::EvenlySpreadVectors(Direction, FVector::ZAxisVector, AngleSpread, SpawnApertures);
	}
	TimerDelegate.BindUObject(this, &AAuraSpawnGate::SpawnEnemy);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, SpawnInterval, true);
}

TArray<FVector> AAuraSpawnGate::CreateSinglePath(const TArray<AActor*>& InActors, const FVector& InOriginalPoint, const FVector& InFinalPoint)
{
	TArray<FVector> Result;
	TArray<AActor*> FoundActors = InActors;
	FVector const Origin = InOriginalPoint;
	FVector const Final = InFinalPoint;
	Result.Add(Origin);
	for (int32 Index=0;Index<FoundActors.Num();Index++)
	{
		if (FMath::FRandRange(0.f,1.f) < 0.7f) // this 0.7f just to make the path more consistent
		{
			Result.Add(FoundActors[Index]->GetActorLocation());
		}
	}
	Result.Add(Final);
	return Result;
}

void AAuraSpawnGate::SpawnEnemy()
{
	if (EnemiesSpawned < NumberOfEnemiesToSpawn)
	{
		if (SpawnClass)
		{
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			AAuraEnemy* Enemy = GetWorld()->SpawnActorDeferred<AAuraEnemy>(SpawnClass, GetActorTransform());
			Enemy->SetLevel(1); //TODO: Change Level according to progression on the game
			Enemy->SetCharacterClass(CharacterClass);
			Enemy->SetPath(GetEnemyPath());
			Enemy->FinishSpawning(GenerateRandomTransform());
			Enemy->SpawnDefaultController();
			EnemiesSpawned++;
		}
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	}
}

FTransform AAuraSpawnGate::GenerateRandomTransform()
{
	FTransform Result;
	const int32 Index = FMath::RandRange(0, SpawnLocations.Num()-1);
	FVector Location = GetActorLocation() + SpawnLocations[Index]*DistanceFromGate;
	Location +=FVector(0.f,0.f,60.f);// todo: could be fixed with height of each enemy
	Result.SetLocation(Location);
	Result.SetRotation(SpawnLocations[Index].ToOrientationQuat());
	return Result;
}

TArray<FVector> AAuraSpawnGate::GetEnemyPath()
{
	const int32 RandPathIndex = FMath::RandRange(0, PathsByPoint.Num()-1);
	return PathsByPoint[RandPathIndex];
}
