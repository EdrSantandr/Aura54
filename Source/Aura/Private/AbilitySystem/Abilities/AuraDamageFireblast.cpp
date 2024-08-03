// EdrSantandr Dev


#include "AbilitySystem/Abilities/AuraDamageFireblast.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actor/AuraFireball.h"

FString UAuraDamageFireblast::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
			"<Title>FIRE BLAST</>\n\n"
			"<Small>Level: </><Level>%d</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
			"<Default>Launches %d fire balls in all directions, each coming back and exploding upon return, causing </>"
			"<Damage>%d</><Default> radial fire damage with a chance to burn</>"
			),
			Level, ManaCost, Cooldown, NumFireBalls, ScaledDamage);
}

FString UAuraDamageFireblast::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
			"<Title>NEXT LEVEL</>\n\n"
			"<Small>Level: </><Level>%d</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
			"<Default>Launches %d fire balls in all directions, each coming back and exploding upon return, causing </>"
			"<Damage>%d</><Default> radial fire damage with a chance to burn</>"
			),
			Level, ManaCost, Cooldown, NumFireBalls, ScaledDamage);
}

TArray<AAuraFireball*> UAuraDamageFireblast::SpawnFireBalls()
{
	TArray<AAuraFireball*> Fireballs;
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	TArray<FRotator> Rotators = UAuraAbilitySystemLibrary::EvenlySpreadRotators(Forward, FVector::UpVector, 360.f, NumFireBalls);
	for (const FRotator& Rotator : Rotators)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(Location);
		SpawnTransform.SetRotation(Rotator.Quaternion());
		AAuraFireball* Fireball = GetWorld()->SpawnActorDeferred<AAuraFireball>(
			FireballClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			CurrentActorInfo->PlayerController->GetPawn(),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		Fireball->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		Fireball->ReturnToActor = GetAvatarActorFromActorInfo();
		Fireball->SetOwner(GetAvatarActorFromActorInfo());
		/*Adding params for explosion*/
		Fireball->ExplosionDamageParams = MakeDamageEffectParamsFromClassDefaults();
		
		Fireballs.Add(Fireball);
		Fireball->FinishSpawning(SpawnTransform);
	}
	return Fireballs;
}
