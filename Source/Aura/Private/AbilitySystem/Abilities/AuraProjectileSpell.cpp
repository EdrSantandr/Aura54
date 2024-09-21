// EdrSantander Dev


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/AuraProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interaction/CombatInterface.h"

bool UAuraProjectileSpell::CheckAbilityRange(const FVector InTargetLocation)
{
	return (InTargetLocation - GetAvatarActorFromActorInfo()->GetActorLocation()).Length() <= ProjectileMaxRange;
}

FVector UAuraProjectileSpell::GetSpawnProjectileLocationPoint(const FVector InActorLocation, const FVector InEnemyLocation, const float InDistance)
{
	FVector Direction = InEnemyLocation - InActorLocation;
	Direction.Normalize();
	Direction *=InDistance;
	return InActorLocation + Direction;
}

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UAuraProjectileSpell::SpawnProjectile(const FVector& TargetLocation,const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;
	
	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);
	FRotator Rotation = (TargetLocation - SocketLocation).Rotation();
	if (bOverridePitch)
	{
		Rotation.Pitch = PitchOverride;
	}
	
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SocketLocation);
	SpawnTransform.SetRotation(Rotation.Quaternion());
		
	AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	
	Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
	
	Projectile->FinishSpawning(SpawnTransform);
}

void UAuraProjectileSpell::SpawnProjectileAtLocation(AActor* HomingTarget, const FVector& SpawnLocation, bool bOverridePitch, float PitchOverride)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;
	const FVector ProjectileTargetLocation = HomingTarget->GetActorLocation();
	FRotator Rotation = (ProjectileTargetLocation - SpawnLocation).Rotation();
	 
	if (bOverridePitch)
	{
		Rotation.Pitch = PitchOverride;
	}
	
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SpawnLocation);
	SpawnTransform.SetRotation(Rotation.Quaternion());
		
	AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
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
	Projectile->ProjectileMovementComponent->bIsHomingProjectile = true;
	
	Projectile->FinishSpawning(SpawnTransform);
}

AAuraProjectile* UAuraProjectileSpell::SpawnProjectileNoTarget(const FVector& SpawnLocation, const FVector PositionAdjustment)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return nullptr;

	const FRotator Rotation =GetAvatarActorFromActorInfo()->GetActorRotation();
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SpawnLocation + PositionAdjustment);
	SpawnTransform.SetRotation(Rotation.Quaternion());
		
	AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	
	Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
	Projectile->ProjectileMovementComponent->Deactivate();
	Projectile->ProjectileMovementComponent->bIsHomingProjectile = false;
	Projectile->FinishSpawning(SpawnTransform);

	return Projectile;
}

void UAuraProjectileSpell::UpdateProjectileTarget(AAuraProjectile* InProjectile, FVector InProjectileTargetLocation)
{
	InProjectile->ProjectileMovementComponent->Activate();
	InProjectile->ProjectileMovementComponent->bIsHomingProjectile = true;
	InProjectile->HomingTargetSceneComponent = NewObject<USceneComponent>(USceneComponent::StaticClass());
	InProjectile->HomingTargetSceneComponent->SetWorldLocation(InProjectileTargetLocation);
	InProjectile->ProjectileMovementComponent->HomingTargetComponent= InProjectile->HomingTargetSceneComponent;
	InProjectile->ProjectileMovementComponent->HomingAccelerationMagnitude = FMath::RandRange(HomingAccelerationMin,HomingAccelerationMax);
}


	
