// EdrSantandr Dev


#include "AbilitySystem/Abilities/AuraEnchantAbility.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerController.h"

void UAuraEnchantAbility::ChangeToConfirmInputMapping()
{
	UAuraAbilitySystemLibrary::ChangeToConfirmImc(this);
}

void UAuraEnchantAbility::ChangeToLiriaInputMapping()
{
	UAuraAbilitySystemLibrary::ChangeToLiriaImc(this);
}
