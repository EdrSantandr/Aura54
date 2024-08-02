// EdrSantandr Dev


#include "Actor/AuraPointCollection.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

AAuraPointCollection::AAuraPointCollection()
{
	PrimaryActorTick.bCanEverTick = false;
	Pt_0 = CreateDefaultSubobject<USceneComponent>("Pt_0");
	SetRootComponent(Pt_0);
	ImmutablePts.Add(Pt_0);

	Pt_1 = CreateDefaultSubobject<USceneComponent>("Pt_1");
	ImmutablePts.Add(Pt_1);
	Pt_1->SetupAttachment(GetRootComponent());

	Pt_2 = CreateDefaultSubobject<USceneComponent>("Pt_2");
	ImmutablePts.Add(Pt_2);
	Pt_2->SetupAttachment(GetRootComponent());

	Pt_3 = CreateDefaultSubobject<USceneComponent>("Pt_3");
	ImmutablePts.Add(Pt_3);
	Pt_3->SetupAttachment(GetRootComponent());
	
	Pt_4 = CreateDefaultSubobject<USceneComponent>("Pt_4");
	ImmutablePts.Add(Pt_4);
	Pt_4->SetupAttachment(GetRootComponent());

	Pt_5 = CreateDefaultSubobject<USceneComponent>("Pt_5");
	ImmutablePts.Add(Pt_5);
	Pt_5->SetupAttachment(GetRootComponent());

	Pt_6 = CreateDefaultSubobject<USceneComponent>("Pt_6");
	ImmutablePts.Add(Pt_6);
	Pt_6->SetupAttachment(GetRootComponent());

	Pt_7 = CreateDefaultSubobject<USceneComponent>("Pt_7");
	ImmutablePts.Add(Pt_7);
	Pt_7->SetupAttachment(GetRootComponent());

	Pt_8 = CreateDefaultSubobject<USceneComponent>("Pt_8");
	ImmutablePts.Add(Pt_8);
	Pt_8->SetupAttachment(GetRootComponent());

	Pt_9 = CreateDefaultSubobject<USceneComponent>("Pt_9");
	ImmutablePts.Add(Pt_9);
	Pt_9->SetupAttachment(GetRootComponent());

	Pt_10 = CreateDefaultSubobject<USceneComponent>("Pt_10");
	ImmutablePts.Add(Pt_10);
	Pt_10->SetupAttachment(GetRootComponent());
	
}

TArray<USceneComponent*> AAuraPointCollection::GetGroundPoints(const FVector& GroundLocation, int32 NumPoints, float YawOverride)
{
	checkf(ImmutablePts.Num() >= NumPoints, TEXT("Attempted to access Immutable out of bouns"));
	TArray<USceneComponent*> ArrayCopy;
	for(USceneComponent* Point : ImmutablePts)
	{
		if (ArrayCopy.Num() >= NumPoints) return ArrayCopy;
		if (Point != Pt_0)
		{
			FVector ToPoint = Point->GetComponentLocation() - Pt_0->GetComponentLocation();
			ToPoint = ToPoint.RotateAngleAxis(YawOverride, FVector::UpVector);
			Point->SetWorldLocation(Pt_0->GetComponentLocation() + ToPoint);
		}

		const FVector RaisedLocation = FVector(Point->GetComponentLocation().X,Point->GetComponentLocation().Y,Point->GetComponentLocation().Z + 500.f);
		const FVector LoweredLocation = FVector(Point->GetComponentLocation().X,Point->GetComponentLocation().Y,Point->GetComponentLocation().Z - 500.f);

		FHitResult HitResult;
		TArray<AActor*> IgnoreActors;
		UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(this, IgnoreActors, TArray<AActor*>(), 1500.f, GetActorLocation());

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActors(IgnoreActors);
		GetWorld()->LineTraceSingleByProfile(HitResult, RaisedLocation, LoweredLocation,FName("BlockAll"),QueryParams);

		const FVector AdjustedLocation = FVector(Point->GetComponentLocation().X, Point->GetComponentLocation().Y, HitResult.ImpactPoint.Z);
		Point->SetWorldLocation(AdjustedLocation);
		Point->SetWorldRotation(UKismetMathLibrary::MakeRotFromZ(HitResult.ImpactNormal));

		ArrayCopy.Add(Point);
	}
	return ArrayCopy;
}

void AAuraPointCollection::BeginPlay()
{
	Super::BeginPlay();
	
}
