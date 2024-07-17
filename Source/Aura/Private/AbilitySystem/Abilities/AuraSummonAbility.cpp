// EdrSantandr Dev


#include "AbilitySystem/Abilities/AuraSummonAbility.h"

#include "Kismet/KismetSystemLibrary.h"

TArray<FVector> UAuraSummonAbility::GetSpawnLocations()
{
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	const float DeltaSpread = SpawnSpread / (NumMinions-1);

	const FVector RightOfSpread = Forward.RotateAngleAxis(SpawnSpread/2.f, FVector::UpVector);
	TArray<FVector> SpawnLocations;
	for (int32 i = 0; i< NumMinions; i++)
	{
		const FVector Direction = RightOfSpread.RotateAngleAxis(-DeltaSpread * i, FVector::UpVector);
		const FVector ChosenSpawnLocation = Location+Direction*FMath::RandRange(MinSpawnDistance, MaxSpawnDistance);
		SpawnLocations.Add(ChosenSpawnLocation);
		UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Location, Location+Direction*MaxSpawnDistance,4.f, FColor::Green, 3.f);
		UKismetSystemLibrary::DrawDebugSphere(GetAvatarActorFromActorInfo(), ChosenSpawnLocation, 20.f, 12, FColor::Yellow, 3.f);
	}
	return SpawnLocations;
}
