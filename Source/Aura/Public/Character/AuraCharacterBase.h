// EdrSantander Dev

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AuraCharacterBase.generated.h"

class UPassiveNiagaraComponent;
class UDeBuffNiagaraComponent;
class UNiagaraSystem;
class UGameplayAbility;
class UGameplayEffect;
class UAttributeSet;
class UAbilitySystemComponent;
class UAnimMontage;

UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();
	virtual void Tick(float DeltaSeconds) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// Each actor can override this function
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet()const {return AttributeSet; }

	/* Combat Interface*/
	// Die function will handle things on the server only
	UFUNCTION(BlueprintCallable, Category="Die")
	virtual void Die(const FVector& DeathImpulse) override;
	virtual bool IsDead_Implementation() const override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) override;
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() override;
	virtual UNiagaraSystem* GetBloodEffect_Implementation() override;
	virtual FTaggedMontage GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag) override;
	virtual int32 GetMinionCount_Implementation() override;
	virtual void IncrementMinionCount_Implementation(int32 AdditionalMinions) override;
	virtual ECharacterClass GetCharacterClass_Implementation() override;
	virtual FOnAscRegisteredSignature& GetOnAscRegisteredDelegate() override;
	virtual FOnDeathSignature& GetOnDeathDelegate() override;
	virtual USkeletalMeshComponent* GetWeapon_Implementation() override;
	virtual bool GetIsBeingShocked_Implementation() const override;
	virtual void SetIsBeingShocked_Implementation(bool bInShock) override;
	virtual FOnDamageSignature& GetOnDamageDelegate() override;
	virtual bool GetIsCasting_Implementation() const override;
	virtual void SetIsCasting_Implementation(bool bInCasting) override;
	virtual bool GetIsSlowed_Implementation() const override;
	virtual void SetIsSlowed_Implementation(bool bInSlowed) override;
	virtual bool GetIsDisarmed_Implementation() const override;
	virtual void SetIsDisarmed_Implementation(bool bInDisarmed) override;
	virtual void ForceDeath_Implementation() override;
	
	
	FOnAscRegisteredSignature OnAscRegisteredDelegate;
	FOnDeathSignature OnDeathDelegate;
	FOnDamageSignature OnDamageDelegate;
	
	// This MulticastHandleDeath will handle death on server and client 
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath(const FVector& DeathImpulse);

	//This Array holds relations between montages and attack gameplay tags
	UPROPERTY(EditAnywhere, Category="Combat")
	TArray<FTaggedMontage> AttackMontages;

	UPROPERTY(ReplicatedUsing=OnRep_Casting, BlueprintReadOnly)
	bool bIsCasting = false;
	
	UPROPERTY(ReplicatedUsing=OnRep_Stunned, BlueprintReadOnly)
	bool bIsStunned = false;

	UPROPERTY(ReplicatedUsing=OnRep_Slowed, BlueprintReadOnly)
	bool bIsSlowed = false;

	UPROPERTY(ReplicatedUsing=OnRep_Burned, BlueprintReadOnly)
	bool bIsBurned = false;

	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bIsBeingShocked = false;

	UPROPERTY(ReplicatedUsing=OnRep_Disarmed, BlueprintReadOnly)
	bool bIsDisarmed = false;

	UFUNCTION()
	virtual void OnRep_Slowed();
	
	UFUNCTION()
	virtual void OnRep_Casting();
	
	UFUNCTION()
	virtual void OnRep_Stunned();

	UFUNCTION()
	virtual void OnRep_Burned();

	UFUNCTION()
	virtual void OnRep_Disarmed();

	UFUNCTION()
	virtual void SetCharacterClass(ECharacterClass InClass);
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(EditAnywhere, Category="Combat")
	FName WeaponTipSocketName;
	
	UPROPERTY(EditAnywhere, Category="Combat")
	FName LeftHandSocketName;

	UPROPERTY(EditAnywhere, Category="Combat")
	FName RightHandSocketName;
	
	UPROPERTY(EditAnywhere, Category="Combat")
	FName TailSocketName;

	UPROPERTY(BlueprintReadOnly)
	bool bDead = false;

	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	virtual void SlowTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	float BaseWalkSpeed = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	float SlowPercentage = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	float GroundFrictionPercentage = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	float AttackRange = 1000.f;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	virtual void InitAbilityActorInfo();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultResistanceAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;
	
	void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffectClass, float Level) const;
	virtual void InitializeDefaultAttributes() const;
	void AddCharacterAbilities() const;

	//Dissolve effect
	virtual void Dissolve();

	UFUNCTION(BlueprintImplementableEvent)
	void StartMeshDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);
	
	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);

	UFUNCTION(BlueprintImplementableEvent)
	void StartDisarmEffectTimeline(float InDuration, bool bActivateEffect = true);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	UNiagaraSystem* BloodEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	USoundBase* DeathSound;

	int32 MinionCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character Class Defaults")
	TObjectPtr<UDecalComponent> CharacterDecal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	FGameplayTag CharacterDecalType;

	UPROPERTY(VisibleAnywhere, Category="Character Debuff Effect")
	TObjectPtr<UDeBuffNiagaraComponent> BurnDeBuffComponent;

	UPROPERTY(VisibleAnywhere, Category="Character Debuff Effect")
	TObjectPtr<UDeBuffNiagaraComponent> StunDeBuffComponent;

	UPROPERTY(VisibleAnywhere, Category="Character Debuff Effect")
	TObjectPtr<UDeBuffNiagaraComponent> PoisonDeBuffComponent;

	UPROPERTY(VisibleAnywhere, Category="Character Debuff Effect")
	TObjectPtr<UDeBuffNiagaraComponent> ArcaneDeBuffComponent;

	UPROPERTY(VisibleAnywhere, Category="Character Debuff Effect")
	TObjectPtr<UDeBuffNiagaraComponent> PhysicalDeBuffComponent;

	UPROPERTY(VisibleAnywhere, Category="Character Debuff Effect")
	TObjectPtr<UDeBuffNiagaraComponent> DisarmDeBuffComponent;
	
private:

	UPROPERTY()
	float OriginalBDWalking = 0.f;
	
	UPROPERTY(EditAnywhere, Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditAnywhere, Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;
	
	UPROPERTY(EditAnywhere, Category="Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY(VisibleAnywhere, Category="Character Passive Effect")
	TObjectPtr<UPassiveNiagaraComponent> HaloOfProtectionNiagaraComponent;

	UPROPERTY(VisibleAnywhere, Category="Character Passive Effect")
	TObjectPtr<UPassiveNiagaraComponent> LifeSiphonNiagaraComponent;

	UPROPERTY(VisibleAnywhere, Category="Character Passive Effect")
	TObjectPtr<UPassiveNiagaraComponent> ManaSiphonNiagaraComponent;

	UPROPERTY(VisibleAnywhere, Category="Character Passive Effect")
	TObjectPtr<USceneComponent> EffectAttachComponent;
};
