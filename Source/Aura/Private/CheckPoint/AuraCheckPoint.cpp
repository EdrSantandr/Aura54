// EdrSantandr Dev


#include "CheckPoint/AuraCheckPoint.h"

#include "Aura/Aura.h"
#include "Components/SphereComponent.h"
#include "Game/AuraGameModeBase.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"

AAuraCheckPoint::AAuraCheckPoint(const FObjectInitializer& ObjectInitializer) : Super (ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	CheckPointMesh = CreateDefaultSubobject<UStaticMeshComponent>("CheckPointMesh");
	CheckPointMesh->SetupAttachment(GetRootComponent());
	CheckPointMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CheckPointMesh->SetCollisionResponseToAllChannels(ECR_Block);
	CheckPointMesh->SetCustomDepthStencilValue(CustomDepthStencilOverride);
	CheckPointMesh->MarkRenderStateDirty();
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetupAttachment(CheckPointMesh);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	MoveToComponent= CreateDefaultSubobject<USceneComponent>("MoveToComponent");
	MoveToComponent->SetupAttachment(GetRootComponent());
}

void AAuraCheckPoint::HighlightActor_Implementation()
{
	if (!bReached)
	{
		CheckPointMesh->SetRenderCustomDepth(true);
	}
}

void AAuraCheckPoint::UnHighlightActor_Implementation()
{
	CheckPointMesh->SetRenderCustomDepth(false);
}

void AAuraCheckPoint::SetMoveToLocation_Implementation(FVector& OutDestination)
{
	OutDestination = MoveToComponent->GetComponentLocation();
}

bool AAuraCheckPoint::ShouldLoadTransform_Implementation()
{
	return false;
}

void AAuraCheckPoint::LoadActor_Implementation()
{
	if (bReached)
	{
		HandleGlowEffects();
	}
}

void AAuraCheckPoint::BeginPlay()
{
	Super::BeginPlay();
	if (bBindCallOverlapCallback)
	{
		SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AAuraCheckPoint::OnBeginOverlap);	
	}
}

void AAuraCheckPoint::HandleGlowEffects()
{
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	UMaterialInstanceDynamic* DynamicMaterialInstance = UMaterialInstanceDynamic::Create(CheckPointMesh->GetMaterial(0),this);
	CheckPointMesh->SetMaterial(0, DynamicMaterialInstance);
	CheckPointReach(DynamicMaterialInstance);
}

void AAuraCheckPoint::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->Implements<UPlayerInterface>())
	{
		bReached = true;
		if (AAuraGameModeBase* AuraGm = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			const UWorld* World = GetWorld();
			FString MapName = World->GetMapName();
			MapName.RemoveFromStart(World->StreamingLevelsPrefix);
			AuraGm->SaveWorldState(GetWorld(), MapName);
		}
		IPlayerInterface::Execute_SaveProgress(OtherActor, PlayerStartTag);
		HandleGlowEffects();
	}
}
