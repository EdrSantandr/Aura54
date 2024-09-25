// EdrSantandr Dev


#include "AbilitySystem/Abilities/AuraPushback.h"

FString UAuraPushback::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
			"<Title>PUSHBACK</>\n\n"
			"<Small>Level: </><Level>%d</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
			"<Default>Creates wind shield dealing </>"
			"<Damage>%d</><Default> radial damage around Liria throwing away the enemies.</>"
			),
			Level, ManaCost, Cooldown, ScaledDamage);
}

FString UAuraPushback::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
			"<Title>NEXT LEVEL</>\n\n"
			"<Small>Level: </><Level>%d</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
			"<Default>Creates wind shield dealing </>"
			"<Damage>%d</><Default> radial damage around Liria throwing away the enemies.</>"
			),
			Level, ManaCost, Cooldown, ScaledDamage);
}
