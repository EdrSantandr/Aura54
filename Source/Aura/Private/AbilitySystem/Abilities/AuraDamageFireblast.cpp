// EdrSantandr Dev


#include "AbilitySystem/Abilities/AuraDamageFireblast.h"

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
	return TArray<AAuraFireball*>();
}
