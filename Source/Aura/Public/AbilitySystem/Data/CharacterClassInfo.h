// EdrSantander Dev

#pragma once

#include "CoreMinimal.h"
#include "ScalableFloat.h"
#include "Engine/DataAsset.h"
#include "CharacterClassInfo.generated.h"

class UGameplayAbility;
class UGameplayEffect;

UENUM()
enum class ECharacterClass :uint16
{
	Elementalist,
	Warrior,
	Ranger,
	SentinelFire,
	SentinelPhysical,
	SentinelArcane,
	SentinelLightning,
	SentinelPoison,
	SentinelWind,
	SentinelBase,
	GateBase,
	SentinelOverPowered
};

USTRUCT()
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category="Class defaults")
	TSubclassOf<UGameplayEffect> PrimaryAttributes;

	UPROPERTY(EditDefaultsOnly, Category="Class defaults")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;

	UPROPERTY(EditDefaultsOnly, Category="Class defaults")
	TObjectPtr<UMaterialInterface> DecalMaterial = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="Class defaults")
	TSubclassOf<AActor> VisualEffect = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="Class defaults")
	TArray<TSubclassOf<UGameplayAbility>> CharacterClassAbilities;

	UPROPERTY(EditDefaultsOnly, Category="Class defaults")
	FScalableFloat XPReward = FScalableFloat();
};

/**
 * 
 */
UCLASS()
class AURA_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly, Category="Common Class defaults")
	TMap<ECharacterClass, FCharacterClassDefaultInfo> CharacterClassInformation;

	UPROPERTY(EditDefaultsOnly, Category="Allies Common Class defaults")
	TSubclassOf<UGameplayEffect> AlliesResistanceAttributes;

	UPROPERTY(EditDefaultsOnly, Category="Allies Common Class defaults")
	TSubclassOf<UGameplayEffect> AlliesVitalAttributes;

	UPROPERTY(EditDefaultsOnly, Category="Allies Common Class defaults")
	TSubclassOf<UGameplayEffect> AlliesSecondaryAttributes;
	
	UPROPERTY(EditDefaultsOnly, Category="Enemies Common Class defaults")
	TSubclassOf<UGameplayEffect> ResistanceAttributes;

	UPROPERTY(EditDefaultsOnly, Category="Enemies Common Class defaults")
	TSubclassOf<UGameplayEffect> VitalAttributes;

	UPROPERTY(EditDefaultsOnly, Category="Enemies Common Class defaults")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;

	UPROPERTY(EditDefaultsOnly, Category="Common Class defaults")
	TSubclassOf<UGameplayEffect> ResistanceAttributesInfinite;
    
	UPROPERTY(EditDefaultsOnly, Category="Common Class defaults")
	TSubclassOf<UGameplayEffect> PrimaryAttributes_SetByCaller;

	UPROPERTY(EditDefaultsOnly, Category="Common Class defaults")
	TSubclassOf<UGameplayEffect> SecondaryAttributesInfinite;

	UPROPERTY(EditDefaultsOnly, Category="Common Class defaults")
	TArray<TSubclassOf<UGameplayAbility>> CommonAbilities;

	FCharacterClassDefaultInfo GetClassDefaultInfo(ECharacterClass CharacterClass);

	UPROPERTY(EditDefaultsOnly, Category="Class defaults|Damage")
	TObjectPtr<UCurveTable> DamageCalculationsCoefficients;
};
