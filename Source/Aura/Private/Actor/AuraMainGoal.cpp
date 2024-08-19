// EdrSantandr Dev


#include "Actor/AuraMainGoal.h"

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
	// todo: Need to retrieve Number of LifePoints from the game mode
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AAuraMainGoal::OnBeginOverlap);
}

void AAuraMainGoal::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Collision with a pawn"));
	//Check if this is an enemy
	//Check the number of lives of each enemy if he gets to the main tree
	//Send the information to the game mode and finish the game
}


