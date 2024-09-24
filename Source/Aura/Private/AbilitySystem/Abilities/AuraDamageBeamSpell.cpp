// EdrSantandr Dev


#include "AbilitySystem/Abilities/AuraDamageBeamSpell.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Character/AuraEnemy.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"

void UAuraDamageBeamSpell::StoreMouseDataInfo(const FHitResult& HitResult)
{
	if (HitResult.bBlockingHit)
	{
		MouseHitLocation = HitResult.ImpactPoint;
		MouseHitActor = HitResult.GetActor();
	}
	else
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	}
}

void UAuraDamageBeamSpell::ElectricStoreMouseDataInfo(AActor* InActor)
{
	if (InActor)
	{
		MouseHitLocation = InActor->GetActorLocation();
		MouseHitActor = InActor;
		if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(MouseHitActor))
		{
			if (!CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this, &UAuraDamageBeamSpell::PrimaryTargetDied))
			{
				CombatInterface->GetOnDeathDelegate().AddDynamic(this, &UAuraDamageBeamSpell::PrimaryTargetDied);
			}
		}
	}
	else
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	}
}

void UAuraDamageBeamSpell::TraceFirstTarget(const FVector& BeamTargetLocation)
{
	check(OwnerCharacter);
	if (OwnerCharacter->Implements<UCombatInterface>())
	{
		if (USkeletalMeshComponent* Weapon = ICombatInterface::Execute_GetWeapon(OwnerCharacter))
		{
			TArray<AActor*> ActorsToIgnore;
			ActorsToIgnore.Add(OwnerCharacter);
			FHitResult HitResult;
			
			const FVector SocketLocation = Weapon->GetSocketLocation(FName("TipSocket"));
			UKismetSystemLibrary::SphereTraceSingle(
				OwnerCharacter,
				SocketLocation,
				BeamTargetLocation,
				10.f,
				TraceTypeQuery1,
				false,
				ActorsToIgnore,
				EDrawDebugTrace::None,
				HitResult,
				true);

			if(HitResult.bBlockingHit)
			{
				MouseHitLocation = HitResult.ImpactPoint;
				MouseHitActor = HitResult.GetActor();
			}
		}
	}
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(MouseHitActor))
	{
		if (!CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this, &UAuraDamageBeamSpell::PrimaryTargetDied))
		{
			CombatInterface->GetOnDeathDelegate().AddDynamic(this, &UAuraDamageBeamSpell::PrimaryTargetDied);
		}
	}
}

void UAuraDamageBeamSpell::SentinelTraceFirstTarget(const FVector& InBeamOriginLocation, const FVector& BeamTargetLocation)
{
	check(OwnerCharacter);
	if (OwnerCharacter->Implements<UCombatInterface>())
	{
		TArray<AActor*> SphereActorsToIgnore;
		TArray<FHitResult> OutHits;
		UKismetSystemLibrary::SphereTraceMulti(GetWorld(), InBeamOriginLocation, BeamTargetLocation, 10.f,TraceTypeQuery1,  false, SphereActorsToIgnore, EDrawDebugTrace::None,OutHits,true);
		for (auto Hit : OutHits)
		{
			UE_LOG(LogTemp, Warning, TEXT("Actor Sphere [%s]"), *Hit.GetActor()->GetName());
		}
		if (OutHits.Num()>0)
		{
			if(const FHitResult HitResult = OutHits.Last(); HitResult.bBlockingHit)
			{
				MouseHitLocation = HitResult.ImpactPoint;
				MouseHitActor = HitResult.GetActor();
				UE_LOG(LogTemp, Warning, TEXT("to affect light Actor [%s]"), *MouseHitActor->GetName());
			}	
		}
	}
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(MouseHitActor))
	{
		if (!CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this, &UAuraDamageBeamSpell::PrimaryTargetDied))
		{
			CombatInterface->GetOnDeathDelegate().AddDynamic(this, &UAuraDamageBeamSpell::PrimaryTargetDied);
		}
	}
}

void UAuraDamageBeamSpell::SentinelSetHitActor(AActor* InActor)
{
	if (IsValid(InActor))
	{
		MouseHitLocation = InActor->GetActorLocation();
		MouseHitActor = InActor;
	}
}

void UAuraDamageBeamSpell::StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets)
{
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetAvatarActorFromActorInfo());
	ActorsToIgnore.Add(MouseHitActor);
	
	TArray<AActor*> OverlappingActors;
	UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(
		GetAvatarActorFromActorInfo(),
		OverlappingActors,
		ActorsToIgnore,
		850.f,
		MouseHitActor->GetActorLocation());

	int32 NumAdditionalTargets = FMath::Min(GetAbilityLevel()-1, MaxNumTargets);
	//int32 NumAdditionalTargets = 5; Just for testing
	if(OverlappingActors.Num() <= NumAdditionalTargets)
	{
		OutAdditionalTargets = OverlappingActors;
	}
	else
	{
		Algo::Sort(OverlappingActors, FSortByDistance(MouseHitActor->GetActorLocation()));
		for(int32 i=0;i < NumAdditionalTargets;i++)
		{
			OutAdditionalTargets.Add(OverlappingActors[i]);
		}
	}
	//UAuraAbilitySystemLibrary::GetClosestTargets(NumAdditionalTargets, OverlappingActors, OutAdditionalTargets, MouseHitActor->GetActorLocation());
	for(AActor* Target : OutAdditionalTargets)
	{
		if(ICombatInterface* CombatInterface = Cast<ICombatInterface>(Target))
		{
			if (!CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this, &UAuraDamageBeamSpell::AdditionalTargetDied))
			{
				CombatInterface->GetOnDeathDelegate().AddDynamic(this, &UAuraDamageBeamSpell::AdditionalTargetDied);	
			}
		}
	}
}

