// EdrSantander Dev


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Aura/AuraLogChannels.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"

struct AuraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PoisonResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(WindResistance);
	
	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false); //Defender 
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false); //Defender
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false); //Attacker
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitChance, Source, false); //Attacker
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitDamage, Source, false); //Attacker
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitResistance, Target, false); //Defending
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, FireResistance, Target, false); //Defending
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, LightningResistance, Target, false); //Defending
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArcaneResistance, Target, false); //Defending
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, PhysicalResistance, Target, false); //Defending
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, PoisonResistance, Target, false); //Defending
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, WindResistance, Target, false); //Defending
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
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);

	RelevantAttributesToCapture.Add(DamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().LightningResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArcaneResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().PoisonResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().WindResistanceDef);
	
}

void UExecCalc_Damage::DetermineDeBuff(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectSpec& Spec, FAggregatorEvaluateParameters EvaluateParameters, const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> InTagsToCaptureDefs) const
{
	const FAuraGameplayTags AuraGameplayTags = FAuraGameplayTags::Get();
	for( TTuple<FGameplayTag, FGameplayTag> Pair : AuraGameplayTags.DamageTypesToDeBuffs)
	{
		const FGameplayTag& DamageType = Pair.Key;
		const FGameplayTag& DeBuffType = Pair.Value;
		const float TypeDamage = Spec.GetSetByCallerMagnitude(DamageType, false, -1.f);
		if (TypeDamage > -.5f)
		{
			// Determine if there was a successful DeBuff
			const float SourceDeBuffChance = Spec.GetSetByCallerMagnitude(AuraGameplayTags.DeBuff_Chance, false, -1.f);
			// Capture the resistance for a damage type
			float TargetDeBuffResistance = 0.f;
			const FGameplayTag& ResistanceTag = AuraGameplayTags.DamageTypesToResistances[DamageType];
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(InTagsToCaptureDefs[ResistanceTag], EvaluateParameters, TargetDeBuffResistance);
			TargetDeBuffResistance = FMath::Max<float>(TargetDeBuffResistance, 0.f);
			//Calculate the chance
			const float EffectiveDeBuffChance = SourceDeBuffChance * (100.f - TargetDeBuffResistance) / 100.f;
			const bool bDeBuff = FMath::RandRange(1,100) < EffectiveDeBuffChance;
			if (bDeBuff)
			{
				FGameplayEffectContextHandle ContextHandle = Spec.GetContext();
				UAuraAbilitySystemLibrary::SetIsSuccessfulDeBuff(ContextHandle, bDeBuff);
				UAuraAbilitySystemLibrary::SetDamageType(ContextHandle, DamageType);
				
				const float DeBuffDamage = Spec.GetSetByCallerMagnitude(AuraGameplayTags.DeBuff_Damage,false, -1.f);
				const float DeBuffDuration = Spec.GetSetByCallerMagnitude(AuraGameplayTags.DeBuff_Duration,false, -1.f);
				const float DeBuffFrequency = Spec.GetSetByCallerMagnitude(AuraGameplayTags.DeBuff_Frequency,false, -1.f);

				UAuraAbilitySystemLibrary::SetDeBuffDamage(ContextHandle, DeBuffDamage);
				UAuraAbilitySystemLibrary::SetDeBuffDuration(ContextHandle, DeBuffDuration);
				UAuraAbilitySystemLibrary::SetDeBuffFrequency(ContextHandle, DeBuffFrequency);
				//UE_LOG(LogAura, Error, TEXT("DAMAGE [%f] DUration [%f] frequency [%f]"), DeBuffDamage, DeBuffDuration, DeBuffFrequency);
			}
		}
	}
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{

	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;
	const FAuraGameplayTags Tags = FAuraGameplayTags::Get(); 

	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_Armor, DamageStatics().ArmorDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_BlockChance, DamageStatics().BlockChanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_ArmorPenetration, DamageStatics().ArmorPenetrationDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitChance, DamageStatics().CriticalHitChanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitDamage, DamageStatics().CriticalHitDamageDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitResistance, DamageStatics().CriticalHitResistanceDef);

	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Fire, DamageStatics().FireResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Lightning, DamageStatics().LightningResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Arcane, DamageStatics().ArcaneResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Physical, DamageStatics().PhysicalResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Poison, DamageStatics().PoisonResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Wind, DamageStatics().WindResistanceDef);
	
	const UAbilitySystemComponent* SourceAsc = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetAsc = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceAsc ? SourceAsc->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetAsc ? TargetAsc->GetAvatarActor() : nullptr;

	int32 SourcePlayerLevel = 1;
	if (SourceAvatar->Implements<UCombatInterface>())
	{
		SourcePlayerLevel = ICombatInterface::Execute_GetPlayerLevel(SourceAvatar);
	}
	int32 TargetPlayerLevel = 1;
	if (TargetAvatar->Implements<UCombatInterface>())
	{
		TargetPlayerLevel = ICombatInterface::Execute_GetPlayerLevel(TargetAvatar);
	}

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();
	
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	/******** DeBuff section *******/
	DetermineDeBuff(ExecutionParams, Spec, EvaluateParameters, TagsToCaptureDefs);
	
	/******** CAPTURE THE ATTRIBUTES VALUES *******/
	/* Get Damage set by caller magnitude*/
	float DamageMagnitude = 0.f;
	for (const auto& Pair: FAuraGameplayTags::Get().DamageTypesToResistances)
	{
		const FGameplayTag DamageTypeTag = Pair.Key;
		const FGameplayTag ResistanceTag = Pair.Value;
		checkf(TagsToCaptureDefs.Contains(ResistanceTag), TEXT("TagsToCaptureDefs dies not contain tag: [%s]"), *ResistanceTag.ToString());
		const FGameplayEffectAttributeCaptureDefinition CaptureDefinition = TagsToCaptureDefs[ResistanceTag];
		/* Scale by the resistance of the character*/
		float DamageTypeValue = Spec.GetSetByCallerMagnitude(DamageTypeTag, false);
		if (DamageTypeValue <= 0.f) continue; //Not needed  to continue if less than 0 
		
		float Resistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDefinition, EvaluateParameters, Resistance);
		Resistance = FMath::Clamp(Resistance, 0.f, 100.f);
		DamageTypeValue *= (100.f - Resistance)/100.f;
		/* Scale the damage if it's radial on inner or outer to the origin*/
		if (UAuraAbilitySystemLibrary::IsRadialDamage(EffectContextHandle))
		{
			if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(TargetAvatar))
			{
				CombatInterface->GetOnDamageDelegate().AddLambda(
					[&](float DamageAmount)
					{
						DamageTypeValue = DamageAmount;
					});
			}
			UGameplayStatics::ApplyRadialDamageWithFalloff(
				TargetAvatar,
				DamageTypeValue,
				0.f,
				UAuraAbilitySystemLibrary::GetRadialDamageOrigin(EffectContextHandle),
				UAuraAbilitySystemLibrary::GetRadialDamageInnerRadius(EffectContextHandle),
				UAuraAbilitySystemLibrary::GetRadialDamageOuterRadius(EffectContextHandle),
				1.f,
				UDamageType::StaticClass(),
				TArray<AActor*>(),
				SourceAvatar,
				nullptr);
		}
		
		DamageMagnitude += DamageTypeValue;
	}
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
	/* Capture Critical hit chance Source*/
	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluateParameters, SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max<float>(0.f, SourceCriticalHitChance);
	/* Capture Critical hit damage Source*/
	float SourceCriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, EvaluateParameters, SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max<float>(0.f, SourceCriticalHitDamage);
	/* Capture Critical hit resistance Target*/
	float TargetCriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef, EvaluateParameters, TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max<float>(0.f, TargetCriticalHitResistance);	
	
	/******** GET THE NECESSARY COEFFICIENTS *******/
	const UCharacterClassInfo* CharacterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
	/*Get ArmorPenetration Source from CharacterClassInfo*/
	const FRealCurve* ArmorPenetrationCurve = CharacterClassInfo->DamageCalculationsCoefficients->FindCurve(FName("ArmorPenetration"), FString());
	const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourcePlayerLevel);
	/*Get Armor Target from CharacterClassInfo*/
	const FRealCurve* EffectiveCurve = CharacterClassInfo->DamageCalculationsCoefficients->FindCurve(FName("EffectiveArmor"), FString());
	const float EffectiveArmorCoefficient = EffectiveCurve->Eval(TargetPlayerLevel);
	/*Get CriticalHitResistance Target from CharacterClassInfo*/
	const FRealCurve* CriticalHitResCurve = CharacterClassInfo->DamageCalculationsCoefficients->FindCurve(FName("CriticalHitResistance"), FString());
	const float CriticalHitResCoefficient = CriticalHitResCurve->Eval(TargetPlayerLevel);
	
	/******** MAKE THE LOGIC USING THE VALUES AND COEFFICIENTS *******/
	/* Block chance to reduce damage in a half*/
	const bool bBlocked = FMath::RandRange(1, 100) < TargetBlockChanceMagnitude;
	DamageMagnitude = bBlocked ? DamageMagnitude * 0.5f : DamageMagnitude;
	/* Save the blocked on the context handle*/
	UAuraAbilitySystemLibrary::SetIsBlockedHit(EffectContextHandle, bBlocked);
	
	/* ArmorPenetration Source ignores a percentage of Target's Armor */
	const float EffectiveArmor = TargetArmorMagnitude * (100.f - SourceArmorPenetrationMagnitude * ArmorPenetrationCoefficient)/100.f;
	DamageMagnitude *= (100.f - EffectiveArmor * EffectiveArmorCoefficient)/100.f;

	/* Critical hit calculations*/
	const float EffectiveCriticalHitChance = SourceCriticalHitChance - TargetCriticalHitResistance * CriticalHitResCoefficient;
	const bool bCriticalHit = FMath::RandRange(1, 100) < EffectiveCriticalHitChance;
	DamageMagnitude = bCriticalHit ? DamageMagnitude * 2.f + SourceCriticalHitDamage : DamageMagnitude;
	/* Save the isCritical on the context handle*/
	UAuraAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bCriticalHit);
	
	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, DamageMagnitude);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
