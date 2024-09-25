// EdrSantandr Dev


#include "AbilitySystem/Abilities/AuraWindWave.h"

FString UAuraWindWave::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
			"<Title>WINDWAVE</>\n\n"
			"<Small>Level: </><Level>%d</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
			"<Default>Create a windwave that gets bigger dealing </>"
			"<Damage>%d</><Default> damage to each enemy trespassed with a huge chance to apply a debuff dealing damage over time.</>"
			),
			Level, ManaCost, Cooldown, ScaledDamage);
}

FString UAuraWindWave::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
			"<Title>NEXT LEVEL</>\n\n"
			"<Small>Level: </><Level>%d</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
			"<Default>Create a windwave that gets bigger dealing </>"
			"<Damage>%d</><Default> damage to each enemy trespassed with a huge chance to apply a debuff dealing damage over time.</>"
			),
			Level, ManaCost, Cooldown, ScaledDamage);
}
