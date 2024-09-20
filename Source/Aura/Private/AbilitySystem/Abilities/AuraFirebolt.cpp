// EdrSantandr Dev


#include "AbilitySystem/Abilities/AuraFirebolt.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actor/AuraProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

FString UAuraFirebolt::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	if(Level==1)
	{
		return FString::Printf(TEXT(
			"<Title>FIRE BOLT</>\n\n"
			"<Small>Level: </><Level>%d</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
			"<Default>Launches a bolt of fire, exploding and dealing </>"
			"<Damage>%d</><Default> fire damage with a chance to burn</>"
			),
			Level, ManaCost, Cooldown, ScaledDamage);
	}
	else
	{
		return FString::Printf(TEXT(
			"<Title>FIRE BOLT</>\n\n"
			"<Small>Level: </><Level>%d</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
			"<Default>Launches %d bolts of fire, exploding and dealing </>"
			"<Damage>%d</><Default> fire damage with a chance to burn</>"
			),
			Level, ManaCost, Cooldown, FMath::Min(Level,NumProjectiles), ScaledDamage);
	}
}

FString UAuraFirebolt::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
			"<Title>NEXT LEVEL</>\n\n"
			"<Small>Level: </><Level>%d</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
			"<Default>Launches %d bolts of fire, exploding and dealing </>"
			"<Damage>%d</><Default> fire damage with a chance to burn</>"
			),
			Level, ManaCost, Cooldown, FMath::Min(Level,NumProjectiles), ScaledDamage);
}

void UAuraFirebolt::SpawnProjectiles(const FVector& ProjectileTargetLocation, bool bOverridePitch, float PitchOverride, AActor* HomingTarget, const FVector& SocketLocation)
{
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	if (bOverridePitch) Rotation.Pitch = PitchOverride;
	
	const FVector Forward = Rotation.Vector();
	const int32 EffectiveNumProjectiles = FMath::Min(NumProjectiles, GetAbilityLevel());
	TArray<FRotator>Rotations = UAuraAbilitySystemLibrary::EvenlySpreadRotators(Forward, FVector::UpVector, ProjectileSpread, EffectiveNumProjectiles);

	for(const FRotator& Rot : Rotations)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rot.Quaternion());
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
					ProjectileClass, SpawnTransform, GetOwningActorFromActorInfo(),
					Cast<APawn>(GetOwningActorFromActorInfo()),
					ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		
		if(HomingTarget && HomingTarget->Implements<UCombatInterface>())
		{
			Projectile->ProjectileMovementComponent->HomingTargetComponent = HomingTarget->GetRootComponent();
		}
		else
		{
			Projectile->HomingTargetSceneComponent = NewObject<USceneComponent>(USceneComponent::StaticClass());
			Projectile->HomingTargetSceneComponent->SetWorldLocation(ProjectileTargetLocation);
			Projectile->ProjectileMovementComponent->HomingTargetComponent= Projectile->HomingTargetSceneComponent;
		}
		Projectile->ProjectileMovementComponent->HomingAccelerationMagnitude = FMath::RandRange(HomingAccelerationMin,HomingAccelerationMax);
		Projectile->ProjectileMovementComponent->bIsHomingProjectile = bLaunchHomingProjectiles;
		Projectile->BindTargetDestroy(HomingTarget);
		Projectile->FinishSpawning(SpawnTransform);
		
	}
}

void UAuraFirebolt::SpawnSpreadProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride, AActor* HomingTarget)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;
	
	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);
	SpawnProjectiles(ProjectileTargetLocation, bOverridePitch, PitchOverride, HomingTarget, SocketLocation);
}

void UAuraFirebolt::SpawnSpreadProjectilesWithOrigin(const FVector& InProjectileTargetLocation, const FVector& InOriginLocation, bool bOverridePitch, float PitchOverride, AActor* HomingTarget)
{
	SpawnProjectiles(InProjectileTargetLocation, bOverridePitch, PitchOverride, HomingTarget, InOriginLocation);
}
