// EdrSantandr Dev


#include "CheckPoint/AuraMapEntrance.h"

#include "Components/SphereComponent.h"
#include "Game/AuraGameModeBase.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"

AAuraMapEntrance::AAuraMapEntrance(const FObjectInitializer& ObjectInitializer)
	: Super (ObjectInitializer)
{
	SphereComponent->SetupAttachment(MoveToComponent);
}

void AAuraMapEntrance::HighlightActor_Implementation()
{
	CheckPointMesh->SetRenderCustomDepth(true);
}

void AAuraMapEntrance::LoadActor_Implementation()
{
	// Do nothing when loading a map entrance;
}

void AAuraMapEntrance::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->Implements<UPlayerInterface>())
	{
		bReached = true;
		if (const AAuraGameModeBase* AuraGm = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			
			AuraGm->SaveWorldState(GetWorld(), DestinationMap.ToSoftObjectPath().GetAssetName());
		}
		IPlayerInterface::Execute_SaveProgress(OtherActor, DestinationPlayerStartTag);

		UGameplayStatics::OpenLevelBySoftObjectPtr(this, DestinationMap);
	}
}
