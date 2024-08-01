// EdrSantandr Dev

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AuraPassiveGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraPassiveGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void ReceivedDeactivate(const FGameplayTag& AbilityTag);
};
