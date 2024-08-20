// EdrSantandr Dev


#include "Actor/AuraPath.h"

#include "Components/BillboardComponent.h"
#include "Components/SphereComponent.h"

AAuraPath::AAuraPath()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SetRootComponent(SphereComponent);

	BillboardComponent = CreateDefaultSubobject<UBillboardComponent>("Billboard");
	BillboardComponent->SetupAttachment(GetRootComponent());

	PrimaryActorTick.bCanEverTick = false;
}

void AAuraPath::BeginPlay()
{
	Super::BeginPlay();
}

