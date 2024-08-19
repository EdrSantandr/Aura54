// EdrSantandr Dev


#include "Actor/AuraMainGoal.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actor/AuraLifePoint.h"
#include "Components/SphereComponent.h"

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

TSubclassOf<AAuraLifePoint> AAuraMainGoal::GetRandomLifePointClass()
{
	const int32 RandomIndex = FMath::RandRange(0, LifePointsClasses.Num()-1);
	return LifePointsClasses[RandomIndex];
}

void AAuraMainGoal::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Collision with a pawn"));
	//Check if this is an enemy
	//Check the number of lives of each enemy if he gets to the main tree
	// Reduce the number of lives
	//Send the information to the game mode and finish the game
}


