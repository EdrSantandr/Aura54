#pragma once

#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"

class UGameplayEffect;

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()

	FDamageEffectParams(){}

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UObject> WorldContextObject = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent;

	UPROPERTY(BlueprintReadWrite)
	float BaseDamage = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float AbilityLevel = 1.f;

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag DamageType = FGameplayTag();

	UPROPERTY(BlueprintReadWrite)
	float DeBuffChance = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float DeBuffDamage = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float DeBuffDuration = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float DeBuffFrequency = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float DeathImpulseMagnitude = 0.f;

	UPROPERTY(BlueprintReadWrite)
	FVector DeathImpulse = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite)
	float KnockBackForceMagnitude = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float KnockBackChance = 0.f;

	UPROPERTY(BlueprintReadWrite)
	FVector KnockBackForce = FVector::ZeroVector;
};

USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY();

public:
	bool IsCriticalHit() const { return bIsCriticalHit; }
	bool IsBlockedHit() const { return bIsBlockedHit; }
	bool IsSuccessfulDeBuff () const { return bIsSuccessfulDeBuff; }
	float GetDeBuffDamage() const {return DeBuffDamage; }
	float GetDeBuffDuration() const {return DeBuffDuration; }
	float GetDeBuffFrequency() const {return DeBuffFrequency; }
	TSharedPtr<FGameplayTag> GetDamageType() const { return DamageType; }
	FVector GetDeathImpulse() const {return DeathImpulse; }
	FVector GetKnockBackForce() const {return KnockBackForce; }

	void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }
	void SetIsBlockedHit(bool bInIsBlockedHit) { bIsBlockedHit = bInIsBlockedHit; }
	void SetIsSuccessfulDeBuff(bool bInIsSuccessfulDeBuff) { bIsSuccessfulDeBuff = bInIsSuccessfulDeBuff; }
	void SetDeBuffDamage(float InDeBuffDamage) { DeBuffDamage = InDeBuffDamage; }
	void SetDeBuffDuration(float InDeBuffDuration) { DeBuffDuration = InDeBuffDuration; }
	void SetDeBuffFrequency(float InDeBuffFrequency) { DeBuffFrequency = InDeBuffFrequency; }
	void SetDamageType(TSharedPtr<FGameplayTag> InDamageType) { DamageType = InDamageType;}
	void SetDeathImpulse(const FVector& InImpulse) { DeathImpulse = InImpulse; }
	void SetKnockBackForce(const FVector& InKnockBackForce) { KnockBackForce = InKnockBackForce; }
	
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return StaticStruct();	
	}

	virtual FAuraGameplayEffectContext* Duplicate() const
	{
		FAuraGameplayEffectContext* NewContext = new FAuraGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override;
protected:

	UPROPERTY()
	bool bIsCriticalHit = false;

	UPROPERTY()
	bool bIsBlockedHit = false;

	UPROPERTY()
	bool bIsSuccessfulDeBuff = false;

	UPROPERTY()
	float DeBuffDamage = 0.f;

	UPROPERTY()
	float DeBuffDuration = 0.f;

	UPROPERTY()
	float DeBuffFrequency = 0.f;
	
	TSharedPtr<FGameplayTag> DamageType;

	UPROPERTY()
	FVector DeathImpulse = FVector::ZeroVector;

	UPROPERTY()
	FVector KnockBackForce = FVector::ZeroVector;
	
};

template <>
struct TStructOpsTypeTraits<FAuraGameplayEffectContext> : TStructOpsTypeTraitsBase2<FAuraGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};
