// EdrSantandr Dev


#include "Actor/AuraMainGoal.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actor/AuraLifePoint.h"
#include "Components/SphereComponent.h"
#include "Interaction/EnemyInterface.h"

// Sets default values
AAuraMainGoal::AAuraMainGoal()
{
	PrimaryActorTick.bCanEverTick = false;

	MainGoalMesh = CreateDefaultSubobject<UStaticMeshComponent>("MainGoalComponent");
	MainGoalMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	SetRootComponent(MainGoalMesh);

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionObjectType(ECC_WorldStatic);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereComponent->SetupAttachment(GetRootComponent());
}

void AAuraMainGoal::BeginPlay()
{
	Super::BeginPlay();
	const int32 NumLives = UAuraAbilitySystemLibrary::GetNumberOfLives(this);
	SpawnLifePoints(NumLives);
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AAuraMainGoal::OnBeginOverlap);
}

void AAuraMainGoal::SpawnLifePoints(int32 NumberOfLives)
{
	TArray<FVector> Directions = UAuraAbilitySystemLibrary::EvenlySpreadVectors(GetActorForwardVector(), FVector::ZAxisVector, 360.f, 20);
	const FVector ForwardActor = GetActorForwardVector();
	for(int Index = 0; Index < NumberOfLives; Index ++)
	{
		FVector Location = GetActorLocation();
		const float RandomDistance = FMath::FRandRange(MinLifePointDistance, MaxLifePointDistance);
		Location += Directions[Index]*RandomDistance;
		FRotator Rotation = ForwardActor.RotateAngleAxis(RandomDistance*Index, FVector::ZAxisVector).Rotation();
		FActorSpawnParameters ActorSpawnParameters;
		ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		AAuraLifePoint* Life = GetWorld()->SpawnActor<AAuraLifePoint>(GetRandomLifePointClass(),Location,Rotation, ActorSpawnParameters);
		LifePoints.Add(Life);
	}
}

void AAuraMainGoal::EliminateLifePoints(const int32 InLifePoints)
{
	const int32 TotalLives = LifePoints.Num();
	for(int32 Index = TotalLives-1; TotalLives-InLifePoints-1 < Index; Index--)
	{
		LifePoints[Index]->DestroyLifePoint();
		LifePoints.RemoveAt(Index);
	}
}

TSubclassOf<AAuraLifePoint> AAuraMainGoal::GetRandomLifePointClass()
{
	const int32 RandomIndex = FMath::RandRange(0, LifePointsClasses.Num()-1);
	return LifePointsClasses[RandomIndex];
}

void AAuraMainGoal::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Collision with a pawn"));
	//TODO: UNCOMMENT THIS SECTION BEFORE SETTING UP ENEMIES BEHAVIOR TREE 
	/*
	if (OtherActor && !OtherActor->Implements<UEnemyInterface>()) return;
	const int32 EnemyDamage = IEnemyInterface::Execute_GetDamagePoints(OtherActor);
	*/
	int32 EnemyDamage = 5;
	int32 CurrentNumLives = UAuraAbilitySystemLibrary::GetNumberOfLives(this);
	if (CurrentNumLives - EnemyDamage < 0)
	{
		EnemyDamage = CurrentNumLives;
	}
	UAuraAbilitySystemLibrary::ReduceNumberOfLives(this, EnemyDamage);
	EliminateLifePoints(EnemyDamage);
	
	CurrentNumLives = UAuraAbilitySystemLibrary::GetNumberOfLives(this);
	if (CurrentNumLives <= 0)
	{
		//todo: if Current Number of lives == 0 do something
		UE_LOG(LogTemp, Warning, TEXT("Should finish the game Numlives [%i]"), CurrentNumLives);
	}
}


