// EdrSantandr Dev


#include "CheckPoint/AuraCheckPoint.h"

#include "Components/SphereComponent.h"

AAuraCheckPoint::AAuraCheckPoint(const FObjectInitializer& ObjectInitializer) : Super (ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	CheckPointMesh = CreateDefaultSubobject<UStaticMeshComponent>("CheckPointMesh");
	CheckPointMesh->SetupAttachment(GetRootComponent());
	CheckPointMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CheckPointMesh->SetCollisionResponseToAllChannels(ECR_Block);
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetupAttachment(CheckPointMesh);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AAuraCheckPoint::BeginPlay()
{
	Super::BeginPlay();
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AAuraCheckPoint::OnBeginOverlap);
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
	if(OtherActor->ActorHasTag(FName("Player")))
	{
		HandleGlowEffects();
	}
}
