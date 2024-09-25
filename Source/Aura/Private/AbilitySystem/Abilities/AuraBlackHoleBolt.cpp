// EdrSantandr Dev


#include "AbilitySystem/Abilities/AuraBlackHoleBolt.h"

FString UAuraBlackHoleBolt::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
			"<Title>BLACK HOLE BOLT</>\n\n"
			"<Small>Level: </><Level>%d</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
			"<Default>Launches a big bolt, attracting enemies dealing</>"
			"<Damage>%d</><Default> at the end, disarming them with a chance to apply an arcane debuff.</>"
			),
			Level, ManaCost, Cooldown, ScaledDamage);
}

FString UAuraBlackHoleBolt::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
			"<Title>NEXT LEVEL</>\n\n"
			"<Small>Level: </><Level>%d</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
			"<Default>Launches a big bolt, attracting enemies dealing</>"
			"<Damage>%d</><Default> at the end, disarming them with a chance to apply an arcane debuff.</>"
			),
			Level, ManaCost, Cooldown, ScaledDamage);
}
