// EdrSantandr Dev


#include "Actor/AuraMagicCircle.h"

#include "Components/DecalComponent.h"

AAuraMagicCircle::AAuraMagicCircle()
{
	PrimaryActorTick.bCanEverTick = true;
	MagicCircleDecal = CreateDefaultSubobject<UDecalComponent>("MagicCircleDecalComponent");
	MagicCircleDecal->SetupAttachment(GetRootComponent());
}

void AAuraMagicCircle::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAuraMagicCircle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

