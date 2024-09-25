// EdrSantander Dev

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Data/CharacterClassInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraAbilitySystemLibrary.generated.h"


struct FGameplayAbilitySpecHandle;
class ULootTiersInfo;
class ULoadScreenSaveGame;
struct FDamageEffectParams;
class UAbilityInfo;
class AAuraHUD;
struct FWidgetControllerParams;
class USpellMenuWidgetController;
struct FGameplayEffectContextHandle;
class UAbilitySystemComponent;
class UAttributeMenuWidgetController;
class UOverlayWidgetController;
/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/* Widget controller*/
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|WidgetController", meta=(DefaultToSelf="WorldContextObject"))
	static bool MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, AAuraHUD*& OutAuraHud);
	
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|WidgetController", meta=(DefaultToSelf="WorldContextObject"))
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|WidgetController", meta=(DefaultToSelf="WorldContextObject"))
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|WidgetController", meta=(DefaultToSelf="WorldContextObject"))
	static USpellMenuWidgetController* GetSpellMenuWidgetController(const UObject* WorldContextObject);

	/* Character class info*/
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|CharacterClassInfo")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* Asc);

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|CharacterClassInfo")
	static void InitializeDefaultAttributesFromSaveData(const UObject* WorldContextObject, UAbilitySystemComponent* Asc, ULoadScreenSaveGame* SaveGame);
	
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|CharacterClassInfo")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* Asc, ECharacterClass CharacterClass, float AbilitiesLevel = 1.f);

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|CharacterClassInfo")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|CharacterClassInfo")
	static UMaterialInterface* GetMaterialDecalByCharacterClass(const UObject* WorldContextObject, ECharacterClass CharacterClass);

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|CharacterClassInfo")
	static TSubclassOf<AActor> GetVisualEffectByCharacterClass(const UObject* WorldContextObject, ECharacterClass CharacterClass);

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|CharacterClassAbilities")
	static void ActivateRandomAbility(UAbilitySystemComponent* InAsc);

	/* Liria Gameplay Parameters*/
	UFUNCTION(BlueprintCallable, Category="LiriaAbilitySystemLibrary|Parameters")
	static int32 GetNumberOfLives(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable,Category="AuraAbilitySystemLibrary|GameplayMechanics")
	static void ReduceNumberOfLives(const UObject* WorldContextObject, const int32 InPoints);

	UFUNCTION(BlueprintCallable,Category="AuraAbilitySystemLibrary|GameplayMechanics")
	static void IncrementNumberOfLives(const UObject* WorldContextObject, const int32 InPoints);
	
	/* Ability info*/
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|AbilityInfo")
	static UAbilityInfo* GetAbilityInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|AbilityInfo")
	static float CalculateManaCostByTag(UAbilitySystemComponent* InAsc, const FGameplayTag& InAbilityTag);

	/* Loot tiers info*/
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|LootTiers", meta=(DefaultToSelf="WorldContextObject"))
	static ULootTiersInfo* GetLootTiersInfo(const UObject* WorldContextObject);

	/* Gameplay Effects contexts getters and setters*/
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static bool IsSuccessfulDeBuff(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static float GetDeBuffDamage(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static float GetDeBuffDuration(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static float GetDeBuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static FGameplayTag GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static FVector GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static FVector GetKnockBackForce(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static bool IsRadialDamage(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static float GetRadialDamageInnerRadius(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static float GetRadialDamageOuterRadius(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static FVector GetRadialDamageOrigin(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable,Category="AuraAbilitySystemLibrary|GameplayEffects")
	static void SetIsBlockedHit(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit);

	UFUNCTION(BlueprintCallable,Category="AuraAbilitySystemLibrary|GameplayEffects")
	static void SetIsCriticalHit(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit);

	UFUNCTION(BlueprintCallable,Category="AuraAbilitySystemLibrary|GameplayEffects")
	static void SetIsSuccessfulDeBuff(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, bool bInIsSuccessfulDeBuff);

	UFUNCTION(BlueprintCallable,Category="AuraAbilitySystemLibrary|GameplayEffects")
	static void SetDeBuffDamage(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, float  InDeBuffDamage);

	UFUNCTION(BlueprintCallable,Category="AuraAbilitySystemLibrary|GameplayEffects")
	static void SetDeBuffDuration(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, float  InDeBuffDuration);

	UFUNCTION(BlueprintCallable,Category="AuraAbilitySystemLibrary|GameplayEffects")
	static void SetDeBuffFrequency(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, float  InDeBuffFrequency);

	UFUNCTION(BlueprintCallable,Category="AuraAbilitySystemLibrary|GameplayEffects")
	static void SetDamageType(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InDamageType);

	UFUNCTION(BlueprintCallable,Category="AuraAbilitySystemLibrary|GameplayEffects")
	static void SetDeathImpulse(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, const FVector& InDeathImpulse);
	
	UFUNCTION(BlueprintCallable,Category="AuraAbilitySystemLibrary|GameplayEffects")
	static void SetKnockBackForce(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, const FVector& InKnockBackForce);

	UFUNCTION(BlueprintCallable,Category="AuraAbilitySystemLibrary|GameplayEffects")
	static void SetIsRadialDamage(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, bool bInIsRadialDamage);

	UFUNCTION(BlueprintCallable,Category="AuraAbilitySystemLibrary|GameplayEffects")
	static void SetRadialDamageInnerRadius(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, float  InRadialDamageInnerRadius);
	
	UFUNCTION(BlueprintCallable,Category="AuraAbilitySystemLibrary|GameplayEffects")
	static void SetRadialDamageOuterRadius(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, float  InRadialDamageOuterRadius);

	UFUNCTION(BlueprintCallable,Category="AuraAbilitySystemLibrary|GameplayEffects")
	static void SetRadialDamageOrigin(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, const FVector& InRadialDamageOrigin);

	/* Gameplay mechanics*/
	UFUNCTION(BlueprintCallable,Category="AuraAbilitySystemLibrary|GameplayMechanics", meta=(DefaultToSelf="WorldContextObject"))
	static void ChangeAllyClass(const UObject* WorldContextObject, const ECharacterClass NewCharacterClass, AActor*& Ally);
	
	UFUNCTION(BlueprintCallable,Category="AuraAbilitySystemLibrary|GameplayMechanics", meta=(DefaultToSelf="WorldContextObject"))
	static AActor* GetHighLightedActor(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable,Category="AuraAbilitySystemLibrary|GameplayMechanics")
	static void ChangeToConfirmImc(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable,Category="AuraAbilitySystemLibrary|GameplayMechanics")
	static void ChangeToLiriaImc(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable,Category="AuraAbilitySystemLibrary|GameplayMechanics")
	static void GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappedActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector SphereOrigin);

	UFUNCTION(BlueprintCallable,Category="AuraAbilitySystemLibrary|GameplayMechanics")
	static void GetLiveEnemiesWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappedActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector SphereOrigin);

	UFUNCTION(BlueprintCallable,Category="AuraAbilitySystemLibrary|GameplayMechanics")
	static void GetPathPointsInsideBox(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappedActors, const TArray<AActor*>& ActorsToIgnore, FVector InOrigin, FVector InFinal, const float XDimension, const float YDimension, const float ZDimension = 500.f);
	
	UFUNCTION(BlueprintCallable,Category="AuraAbilitySystemLibrary|GameplayMechanics")
	static void GetClosestTargets(int32 MaxTargets, const TArray<AActor*>& Actors, TArray<AActor*>& OutClosetsTargets, const FVector& Origin);
	
	UFUNCTION(BlueprintPure,Category="AuraAbilitySystemLibrary|GameplayMechanics")
	static bool IsNotFriend(AActor* FirstActor, AActor* SecondActor);
	
	UFUNCTION(BlueprintPure,Category="AuraAbilitySystemLibrary|GameplayMechanics")
	static TArray<FRotator> EvenlySpreadRotators(const FVector& Forward, const FVector& Axis, float Spread, int32 NumRotators);

	UFUNCTION(BlueprintPure,Category="AuraAbilitySystemLibrary|GameplayMechanics")
	static TArray<FVector> EvenlySpreadVectors(const FVector& Forward, const FVector& Axis, float Spread, int32 NumVectors);

	static int32 GetXpRewardForClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 CharacterLevel);

	/* Damage effect*/
	UFUNCTION(BlueprintCallable,Category="AuraAbilitySystemLibrary|DamageEffect")
	static FGameplayEffectContextHandle ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams);

	UFUNCTION(BlueprintCallable,Category="AuraAbilitySystemLibrary|DamageEffect")
	static void SetIsRadialDamageEffectParam(UPARAM(ref)FDamageEffectParams& DamageEffectParams, bool bIsRadial, float InnerRadius, float OuterRadius, FVector Origin);

	UFUNCTION(BlueprintCallable,Category="AuraAbilitySystemLibrary|DamageEffect")
	static void SetEffectParamsTargetAsc(UPARAM(ref)FDamageEffectParams& DamageEffectParams, UAbilitySystemComponent* InTargetAsc);

	UFUNCTION(BlueprintCallable,Category="AuraAbilitySystemLibrary|DamageEffect")
	static void SetKnockBackDirection(UPARAM(ref)FDamageEffectParams& DamageEffectParams, FVector KnockBackDirection, float Magnitude =-1.f);

	UFUNCTION(BlueprintCallable,Category="AuraAbilitySystemLibrary|DamageEffect")
	static void SetDeathImpulseDirection(UPARAM(ref)FDamageEffectParams& DamageEffectParams, FVector DeathImpulseDirection, float Magnitude =-1.f);
	
};
