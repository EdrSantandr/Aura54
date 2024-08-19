// EdrSantandr Dev


#include "Actor/AuraLifePoint.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

AAuraLifePoint::AAuraLifePoint()
{
	PrimaryActorTick.bCanEverTick = true;

	LifePointMesh = CreateDefaultSubobject<UStaticMeshComponent>("LifePointMeshComponent");
	LifePointMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	SetRootComponent(LifePointMesh);
}

void AAuraLifePoint::DestroyLifePoint()
{
	TimerDelegate.BindUObject(this, &AAuraLifePoint::LifePoint_Elapsed);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, TimeBeforeDestroy, false);
	if (DestroyEffect) NiagaraSpawned = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, DestroyEffect, GetActorLocation());
}

void AAuraLifePoint::Destroyed()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	Super::Destroyed();
}

void AAuraLifePoint::BeginPlay()
{
	Super::BeginPlay();
	const float RandomScale = FMath::FRandRange(MinScale, MaxScale);
	LifePointMesh->SetWorldScale3D(FVector(RandomScale, RandomScale, RandomScale));
}

void AAuraLifePoint::LifePoint_Elapsed()
{
	if (NiagaraSpawned) NiagaraSpawned->Deactivate();
	Destroy();
}
