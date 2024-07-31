// EdrSantandr Dev

#pragma once

#include "CoreMinimal.h"
#include "AuraAbilityTypes.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "Interaction/CombatInterface.h"
#include "AuraDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraDamageGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);

	UFUNCTION(BlueprintPure)
	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor = nullptr) const;

	UFUNCTION(BlueprintPure)
	float GetDamageAtLevel() const;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	FGameplayTag DamageType;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	FScalableFloat Damage;

	UPROPERTY(EditDefaultsOnly, Category="DebBuff Damage")
	FScalableFloat DeBuffChance;

	UPROPERTY(EditDefaultsOnly, Category="DebBuff Damage")
	FScalableFloat DeBuffDamage;

	UPROPERTY(EditDefaultsOnly, Category="DebBuff Damage")
	FScalableFloat DeBuffFrequency;

	UPROPERTY(EditDefaultsOnly, Category="DebBuff Damage")
	FScalableFloat DeBuffDuration;

	UPROPERTY(EditDefaultsOnly, Category="Death")
	float DeathImpulseMagnitude = 10000.f;

	UPROPERTY(EditDefaultsOnly, Category="Knockback")
	float KnockBackForceMagnitude = 1000.f;

	UPROPERTY(EditDefaultsOnly, Category="Knockback")
	float KnockBackChance = 0.f;
	
	UFUNCTION(BlueprintPure)
	FTaggedMontage GetRandomTaggedMontage(const TArray<FTaggedMontage>& TaggedMontages) const;
};
