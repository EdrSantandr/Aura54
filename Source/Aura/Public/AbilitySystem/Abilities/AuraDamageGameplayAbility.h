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
	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(
		AActor* TargetActor = nullptr,
		FVector InRadialDamageOrigin = FVector::ZeroVector,
		bool bOverrideKnockBackDirection = false,
		FVector KnockBackDirectionOverride = FVector::ZeroVector,
		bool bOverrideDeathImpulse = false,
		FVector DeathImpulseDirectionOverride = FVector::ZeroVector,
		bool bOverridePitch = false,
		float PitchOverride = 0.f
		) const;

	UFUNCTION(BlueprintPure)
	float GetDamageAtLevel() const;

	UFUNCTION(BlueprintCallable)
	void ChangeToConfirmInputMapping();

	UFUNCTION(BlueprintCallable)
	void ChangeToLiriaInputMapping();

	UFUNCTION(BlueprintCallable)
	void StoreOwnerVariables();
	
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

	UPROPERTY(EditDefaultsOnly, Category="RadialDamage")
	bool bIsRadialDamage = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="RadialDamage")
	float RadialDamageInnerRadius = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="RadialDamage")
	float RadialDamageOuterRadius = 0.f;
	
	UFUNCTION(BlueprintPure)
	FTaggedMontage GetRandomTaggedMontage(const TArray<FTaggedMontage>& TaggedMontages) const;

	UPROPERTY(BlueprintReadWrite, Category="Beam")
	TObjectPtr<APlayerController> OwnerPlayerController;

	UPROPERTY(BlueprintReadWrite, Category="Beam")
	TObjectPtr<ACharacter> OwnerCharacter;

};
