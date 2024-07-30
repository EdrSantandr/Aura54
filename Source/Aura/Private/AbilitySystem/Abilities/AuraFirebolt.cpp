// EdrSantandr Dev


#include "AbilitySystem/Abilities/AuraFirebolt.h"

#include "Kismet/KismetSystemLibrary.h"

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
			"<Title>Fire Bolt</>\n\n"
			"<Small>Level: </><Level>%d</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
			"<Default>Launches %d bolts of fire, exploding and dealing </>"
			"<Damage>%d</><Default> fire damage with a chance to burn</>"
			),
			Level, ManaCost, Cooldown, FMath::Min(0,NumProjectiles), ScaledDamage);
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
			Level, ManaCost, Cooldown, FMath::Min(0,NumProjectiles), ScaledDamage);
}

void UAuraFirebolt::SpawnSpreadProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride, AActor* HomingTarget)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;
	
	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	if (bOverridePitch) Rotation.Pitch = PitchOverride;
	
	const FVector Forward = Rotation.Vector();
	const FVector LeftOfSpread =  Forward.RotateAngleAxis(-ProjectileSpread/2.f, FVector::UpVector);
	const FVector RightOfSpread =  Forward.RotateAngleAxis(ProjectileSpread/2.f, FVector::UpVector);
	
	//NumProjectiles = FMath::Min(MaxNumProjectiles, GetAbilityLevel());
	
	if (NumProjectiles>1)
	{
		const float DeltaSpread = ProjectileSpread / (NumProjectiles - 1);
		for (int32 i=0; i<NumProjectiles; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread*i, FVector::UpVector);
			UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, SocketLocation + Direction*75.f, 5, FLinearColor::Yellow, 120, 5.f);		
		}
	}
	else
	{
		//Single Projectile	
	}
	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, SocketLocation + Rotation.Vector()*100.f, 1, FLinearColor::White, 120, 1.f);
	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, SocketLocation + LeftOfSpread*100.f, 1, FLinearColor::Blue, 120, 1.f);
	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, SocketLocation + RightOfSpread*100.f, 1, FLinearColor::Red, 120, 1.f);
	
}
