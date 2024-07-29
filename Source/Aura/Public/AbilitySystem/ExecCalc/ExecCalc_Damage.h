// EdrSantander Dev

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ExecCalc_Damage.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UExecCalc_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UExecCalc_Damage();
	void DetermineDeBuff(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	                     const FGameplayEffectSpec& Spec,
	                     FAggregatorEvaluateParameters EvaluateParameters,
	                     const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> InTagsToCaptureDefs) const;

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
