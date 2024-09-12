// EdrSantandr Dev


#include "Actor/AuraMainGoal.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actor/AuraLifePoint.h"
#include "Components/SphereComponent.h"
#include "Game/AuraGameModeBase.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/EnemyInterface.h"
#include "Kismet/GameplayStatics.h"

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
	TArray<FVector> Directions = UAuraAbilitySystemLibrary::EvenlySpreadVectors(GetActorForwardVector(), FVector::ZAxisVector, 360.f, NumberOfLives);
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
	if (OtherActor && !OtherActor->Implements<UEnemyInterface>()) return;
	int32 EnemyDamage = IEnemyInterface::Execute_GetDamagePoints(OtherActor);	
	int32 CurrentNumLives = UAuraAbilitySystemLibrary::GetNumberOfLives(this);
	if (OtherActor->Implements<UCombatInterface>())
		ICombatInterface::Execute_ForceDeath(OtherActor);
	
	if (CurrentNumLives - EnemyDamage < 0)
	{
		EnemyDamage = CurrentNumLives;
	}
	UAuraAbilitySystemLibrary::ReduceNumberOfLives(this, EnemyDamage);
	EliminateLifePoints(EnemyDamage);
	
	CurrentNumLives = UAuraAbilitySystemLibrary::GetNumberOfLives(this);
	if (CurrentNumLives <= 0)
	{
		SetDarkLightning();
		SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if (AAuraGameModeBase* AuraGm = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			if (const APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
			{
				AuraGm->LiriaPlayerDied(PC->GetCharacter());	
			}
		}
	}
}


