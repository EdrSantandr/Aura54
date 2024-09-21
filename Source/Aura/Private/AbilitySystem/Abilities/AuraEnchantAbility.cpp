// EdrSantandr Dev


#include "AbilitySystem/Abilities/AuraEnchantAbility.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"

void UAuraEnchantAbility::EnchantAlly(AActor* InAlly)
{
	UAuraAbilitySystemLibrary::ChangeAllyClass(this,NewCharacterClass, InAlly);
}
