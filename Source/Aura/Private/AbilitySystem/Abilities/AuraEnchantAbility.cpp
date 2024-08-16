// EdrSantandr Dev


#include "AbilitySystem/Abilities/AuraEnchantAbility.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"

void UAuraEnchantAbility::ChangeToConfirmInputMapping()
{
	UAuraAbilitySystemLibrary::ChangeToConfirmImc(this);
}

void UAuraEnchantAbility::ChangeToLiriaInputMapping()
{
	UAuraAbilitySystemLibrary::ChangeToLiriaImc(this);
}

void UAuraEnchantAbility::EnchantAlly(AActor* InAlly)
{
	UAuraAbilitySystemLibrary::ChangeAllyClass(this,NewCharacterClass, InAlly);
}
