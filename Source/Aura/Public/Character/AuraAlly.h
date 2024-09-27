// EdrSantandr Dev

#pragma once

#include "CoreMinimal.h"
#include "Aura/Aura.h"
#include "Character/AuraCharacter.h"
#include "Interaction/AllyInterface.h"
#include "Interaction/HighlightInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AuraAlly.generated.h"

class AAIController;
class AAuraEnemy;
class UWidgetComponent;
class UBehaviorTree;
class AAuraAIController;
/**
 * 
 */

struct FSortVectorByLenght
{
	explicit FSortVectorByLenght(const FVector& InSourceLocation)
		: SourceLocation(InSourceLocation) {}
	
	FVector SourceLocation = FVector::Zero();

	bool operator()(const AActor* A, const AActor* B) const
	{
		float DistanceA = FVector::DistSquared(SourceLocation, A->GetActorLocation());
		float DistanceB = FVector::DistSquared(SourceLocation, B->GetActorLocation());

		return DistanceA < DistanceB;
	}
};
UCLASS()
class AURA_API AAuraAlly : public AAuraCharacterBase, public IHighlightInterface, public IAllyInterface
{
	GENERATED_BODY()

public:
	AAuraAlly();
	virtual void PossessedBy(AController* NewController) override;

	/* Ally Interface*/
	virtual ECharacterClass GetCharacterClass_Implementation() override;
	virtual AActor* GetCombatTargetAlly_Implementation() const override;
	virtual void SetCombatTargetAlly_Implementation(AActor* InCombatTarget) override;
	virtual void SetLevel_Implementation(int32 NewLevel) override;
	virtual bool GetIsGate_Implementation() override;
	
	/* HighlightInterface*/
	virtual void HighlightActor_Implementation() override;
	virtual void UnHighlightActor_Implementation() override;
	virtual void SetMoveToLocation_Implementation(FVector& OutDestination) override;

	/* CombatInterface*/
	virtual int32 GetPlayerLevel_Implementation() override;
	virtual void Die(const FVector& DeathImpulse) override;
	virtual UMeshComponent* GetBeamOriginComponent_Implementation() override;
	
	virtual void MulticastHandleDeath_Implementation(const FVector& DeathImpulse) override;

	virtual void Dissolve() override;

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UFUNCTION(BlueprintCallable, Category="Combat")
	void ShowDecal() const;

	UFUNCTION(BlueprintCallable, Category="Combat")
	void HideDecal() const;
	
	UPROPERTY(BlueprintReadOnly, Category="Combat")
	bool bHitReacting = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	float Lifespan = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	float StencilColorValue = CUSTOM_DEPTH_BLUE;
	
	UPROPERTY(BlueprintReadWrite, Category="Combat")
	TObjectPtr<AActor> CombatTarget;

	UFUNCTION()
	void SetDecalMaterial(UMaterialInterface* InDecalMaterial) const;

	UFUNCTION()
	void SpawnVisualEffectActor();
	
	TArray<TArray<FVector>> GetPathPoints() { return PathsByPoint;}

	UFUNCTION(BlueprintCallable, Category="SpawnGate")
	int32 GetEnemiesToSpawn() const { return NumberOfEnemiesToSpawn; }
	
	UFUNCTION(BlueprintCallable, Category="SpawnGate")
	int32 GetEnemiesSpawned() const { return EnemiesSpawned; }

	UFUNCTION(BlueprintCallable, Category="SpawnGate")
	TArray<FVector> GetSpawnLocations() const { return SpawnLocations; }

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SpawnGate")
	TSubclassOf<AAuraAlly> SentinelSpawnClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SpawnGate")
	ECharacterClass SentinelClass = ECharacterClass::SentinelFire;

	UFUNCTION()
	void SpawnSentinel() const;

	void IncreaseEnemiesSpawned(int32 InAddNumber) { EnemiesSpawned += InAddNumber; }

	virtual void SetCharacterClass(ECharacterClass InClass) override;

	UFUNCTION(BlueprintCallable, Category="AiController")
	void ActivateBehaviorTree(AAIController* InAiController) const;

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount) override;
	virtual void Destroyed() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Class Defaults")
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(BlueprintAssignable)
	FOnValueChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnValueChangedSignature OnMaxHealthChanged;

	UPROPERTY(EditAnywhere, Category="AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Effect")
	float EffectHeight = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Effect")
	bool bIsGate = false;

	UPROPERTY()
	TObjectPtr<AAuraAIController> AuraAIController;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	TObjectPtr<UStaticMeshComponent> BeamOriginComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<AActor> VisualEffectActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="SpawnGate")
	TObjectPtr<AActor> MainGoal;
	
	UPROPERTY(EditDefaultsOnly, Category="SpawnGate")
	int32 NumberOfEnemiesToSpawn = 5;
	
	UPROPERTY(EditDefaultsOnly, Category="SpawnGate")
	float AngleSpread = 90.f;

	UPROPERTY(EditDefaultsOnly, Category="SpawnGate")
	int32 SpawnApertures = 4;

	UPROPERTY(EditDefaultsOnly, Category="SpawnGate")
	float YPathDimension = 1000.f;

private:

	UFUNCTION()
	void CreatePathsFromGate(const int32 NumPaths, const FVector& InOriginalPoint, const FVector& InFinalPoint);
	
	static TArray<FVector> CreateSinglePath(const TArray<AActor*>& InActors, const FVector& InOriginalPoint, const FVector& InFinalPoint);
	
	TArray<TArray<FVector>> PathsByPoint;
	TArray<FVector> SpawnLocations;
	int32 EnemiesSpawned = 0;
};
