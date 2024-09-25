// EdrSantandr Dev


#include "AbilitySystem/Abilities/AuraMeteor.h"

FString UAuraMeteor::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
			"<Title>METEOR</>\n\n"
			"<Small>Level: </><Level>%d</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
			"<Default>Creates a big meteor that deals </>"
			"<Damage>%d</><Default> radial damage when hitting the ground, with a increased chance to apply an fire debuff.</>"
			),
			Level, ManaCost, Cooldown, ScaledDamage);
}

FString UAuraMeteor::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
			"<Title>NEXT LEVEL</>\n\n"
			"<Small>Level: </><Level>%d</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
			"<Default>Creates a big meteor that deals </>"
			"<Damage>%d</><Default> radial damage when hitting the ground, with a increased chance to apply an fire debuff.</>"
			),
			Level, ManaCost, Cooldown, ScaledDamage);
}
