// EdrSantander Dev


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"

struct AuraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	
	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false); //Attacker 
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false); //Attacker
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false); //Defending
	}
};

static const AuraDamageStatics& DamageStatics()
{
	static AuraDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceAsc = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetAsc = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceAsc ? SourceAsc->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetAsc ? TargetAsc->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	/* Get Damage set by caller magnitude*/
	float DamageMagnitude = Spec.GetSetByCallerMagnitude(FAuraGameplayTags::Get().Damage);
	/* Capture BlockChance on target and determine ir there was a successful block*/
	float TargetBlockChanceMagnitude = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluateParameters, TargetBlockChanceMagnitude);
	TargetBlockChanceMagnitude = FMath::Max<float>(0.f, TargetBlockChanceMagnitude);
	/* Capture the Armor Target*/
	float TargetArmorMagnitude = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluateParameters, TargetArmorMagnitude);
	TargetArmorMagnitude = FMath::Max<float>(0.f, TargetArmorMagnitude);
	/* Capture ArmorPenetration Source*/
	float SourceArmorPenetrationMagnitude = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvaluateParameters, SourceArmorPenetrationMagnitude);
	SourceArmorPenetrationMagnitude = FMath::Max<float>(0.f, SourceArmorPenetrationMagnitude);
	/* Block chance to reduce damage in a half*/
	const bool bBlocked = FMath::RandRange(1, 100) < TargetBlockChanceMagnitude;
	DamageMagnitude = bBlocked ? DamageMagnitude * 0.5f : DamageMagnitude;
	/* ArmorPenetration Source ignores a percentage of Target's Armor */
	const float EffectiveArmor = TargetArmorMagnitude *= (100.f - SourceArmorPenetrationMagnitude)/100.f;
	DamageMagnitude *= (100.f - EffectiveArmor)/100.f;
	
	
	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, DamageMagnitude);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
