// EdrSantandr Dev


#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"


void UAuraDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	const FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);
	const float ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, DamageType, ScaledDamage);
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(),
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}

FDamageEffectParams UAuraDamageGameplayAbility::MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor) const
{
	FDamageEffectParams Params;
	Params.WorldContextObject = GetAvatarActorFromActorInfo();
	Params.DamageGameplayEffectClass = DamageEffectClass;
	Params.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	Params.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	Params.BaseDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	Params.AbilityLevel = GetAbilityLevel();
	Params.DamageType = DamageType;
	Params.DeBuffChance = DeBuffChance.GetValueAtLevel(GetAbilityLevel());
	Params.DeBuffDamage = DeBuffDamage.GetValueAtLevel(GetAbilityLevel());
	Params.DeBuffDuration = DeBuffDuration.GetValueAtLevel(GetAbilityLevel());
	Params.DeBuffFrequency = DeBuffFrequency.GetValueAtLevel(GetAbilityLevel());
	return Params;
}

FTaggedMontage UAuraDamageGameplayAbility::GetRandomTaggedMontage(const TArray<FTaggedMontage>& TaggedMontages) const
{
	if(TaggedMontages.Num() > 0)
	{
		const int32 Selection = FMath::RandRange(0, TaggedMontages.Num()-1);
		return TaggedMontages[Selection];
	}
	return FTaggedMontage();
}
