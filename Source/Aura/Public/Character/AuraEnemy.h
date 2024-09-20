// EdrSantander Dev

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "Interaction/HighlightInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AuraEnemy.generated.h"

class UWidgetComponent;
class UBehaviorTree;
class AAuraAIController;
/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface, public IHighlightInterface
{
	GENERATED_BODY()

public:
	AAuraEnemy();
	virtual void PossessedBy(AController* NewController) override;

	/** HighlightInterface */
	virtual void HighlightActor_Implementation() override;
	virtual void UnHighlightActor_Implementation() override;
	virtual void SetMoveToLocation_Implementation(FVector& OutDestination) override;
	
	/** Enemy Interface*/
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	virtual AActor* GetCombatTarget_Implementation() const override;
	virtual int32 GetDamagePoints_Implementation() const override;

	/** Combat Interface*/
	virtual int32 GetPlayerLevel_Implementation() override;
	virtual void Die(const FVector& DeathImpulse) override;
	virtual void StartDisarm_Implementation(float InDuration, bool InShowEffect) override;
	virtual void SetIsBeingShocked_Implementation(bool bInShock) override;

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(BlueprintReadOnly, Category="Combat")
	bool bHitReacting = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	float Lifespan = 3.f;

	UPROPERTY(BlueprintReadWrite, Category="Combat")
	TObjectPtr<AActor> CombatTarget;

	void SetLevel(int32 InLevel) {Level = InLevel;}

	void SetPath(const TArray<FVector>& InPath); 

	UFUNCTION(BlueprintCallable, Category="Liria")
	FVector GetPathPoint();

	UFUNCTION(BlueprintCallable, Category="Liria")
	void IncrementPathPoint();

	UFUNCTION(BlueprintCallable, Category="Liria")
	int32 GetCurrentIndex() const { return CurrentPathIndex; }

	void FinishDisarm();

	void HandleEnemyKilled() const;
	
protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount) override;
	virtual void SlowTagChanged(const FGameplayTag CallbackTag, int32 NewCount) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Class Defaults")
	int32 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	int32 DamageLife = 1;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(BlueprintAssignable)
	FOnValueChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnValueChangedSignature OnMaxHealthChanged;

	UPROPERTY(EditAnywhere, Category="AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AAuraAIController> AuraAIController;

	UFUNCTION(BlueprintImplementableEvent)
	void SpawnLoot();

	UPROPERTY()
	TArray<FVector> PathPoints;

	UPROPERTY()
	int32 CurrentPathIndex = 0;

	FTimerHandle DisarmTimerHandle;
	FTimerDelegate DisarmTimerDelegate;
};
