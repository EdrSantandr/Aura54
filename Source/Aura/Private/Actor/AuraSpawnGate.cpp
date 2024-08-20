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

void AAuraSpawnGate::BeginPlay()
{
	Super::BeginPlay();
	TimerDelegate.BindUObject(this, &AAuraSpawnGate::SpawnEnemy);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, SpawnInterval, true);
	SpawnLocations = UAuraAbilitySystemLibrary::EvenlySpreadVectors(GetActorForwardVector(), FVector::ZAxisVector, AngleSpread, SpawnApertures);
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
			Enemy->FinishSpawning(GenerateRandomTransform());
			Enemy->SpawnDefaultController();	
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Enemy class not set"));
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
