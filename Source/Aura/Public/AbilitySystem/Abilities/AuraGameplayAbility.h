// EdrSantander Dev

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AuraGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	FGameplayTag StartupInputTag;

	UPROPERTY(EditDefaultsOnly, Category="Checking")
	bool bCheckCooldown = false;

	UPROPERTY(EditDefaultsOnly, Category="Checking")
	bool bCheckCost = false;

	virtual FString GetDescription(int32 Level);
	virtual FString GetNextLevelDescription(int32 Level);
	static FString GetLockedDescription(int32 Level);

	UFUNCTION()
	float GetCurrentLevelManaCost() const;
	
	UFUNCTION(BlueprintCallable, Category="PlayerMessage")
	void SendMessageBP(const FGameplayTag& InMessageTag);
	
	void SendMessage(const FGameplayTag& InMessageTag) const;

	UFUNCTION(BlueprintCallable, Category="PlayerMessage")
	void CheckCostAndCooldown(const FGameplayAbilityActorInfo InActorInfo) const;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

protected:
	float GetManaCost(float InLevel = 1.f) const;
	float GetCooldown(float InLevel = 1.f) const;

private:
	
	void SaveCharacterWalkSpeed(const FGameplayAbilityActorInfo* ActorInfo);
	void SetCharacterWalkSpeed(const FGameplayAbilityActorInfo* ActorInfo) const;
	float CharacterWalkSpeed = 0.f;
};
