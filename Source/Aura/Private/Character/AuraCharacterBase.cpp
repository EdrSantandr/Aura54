// EdrSantander Dev


#include "Character/AuraCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/DeBuff/DeBuffNiagaraComponent.h"
#include "AbilitySystem/Passive/PassiveNiagaraComponent.h"
#include "Aura/Aura.h"
#include "Components/CapsuleComponent.h"
#include "Components/DecalComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	BurnDeBuffComponent = CreateDefaultSubobject<UDeBuffNiagaraComponent>("BurnDeBuffComponent");
	BurnDeBuffComponent->SetupAttachment(GetRootComponent());
	BurnDeBuffComponent->DeBuffTag = FAuraGameplayTags::Get().DeBuff_Burn;

	StunDeBuffComponent = CreateDefaultSubobject<UDeBuffNiagaraComponent>("StunDeBuffComponent");
	StunDeBuffComponent->SetupAttachment(GetRootComponent());
	StunDeBuffComponent->DeBuffTag = FAuraGameplayTags::Get().DeBuff_Stun;

	PoisonDeBuffComponent = CreateDefaultSubobject<UDeBuffNiagaraComponent>("PoisonDeBuffComponent");
	PoisonDeBuffComponent->SetupAttachment(GetRootComponent());
	PoisonDeBuffComponent->DeBuffTag = FAuraGameplayTags::Get().DeBuff_Slow;

	ArcaneDeBuffComponent = CreateDefaultSubobject<UDeBuffNiagaraComponent>("ArcaneDeBuffComponent");
	ArcaneDeBuffComponent->SetupAttachment(GetRootComponent());
	ArcaneDeBuffComponent->DeBuffTag = FAuraGameplayTags::Get().DeBuff_Arcane;
	
	PhysicalDeBuffComponent = CreateDefaultSubobject<UDeBuffNiagaraComponent>("PhysicalDeBuffComponent");
	PhysicalDeBuffComponent->SetupAttachment(GetRootComponent());
	PhysicalDeBuffComponent->DeBuffTag = FAuraGameplayTags::Get().DeBuff_Physical;

	DisarmDeBuffComponent = CreateDefaultSubobject<UDeBuffNiagaraComponent>("DisarmDeBuffComponent");
	DisarmDeBuffComponent->SetupAttachment(GetRootComponent());
	DisarmDeBuffComponent->DeBuffTag = FAuraGameplayTags::Get().DeBuff_Disarm;
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_PROJECTILE, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	EffectAttachComponent = CreateDefaultSubobject<USceneComponent>("EffectAttachComponent");
	EffectAttachComponent->SetupAttachment(GetRootComponent());

	HaloOfProtectionNiagaraComponent = CreateDefaultSubobject<UPassiveNiagaraComponent>("HaloOfProtectionComponent");
	HaloOfProtectionNiagaraComponent->SetupAttachment(EffectAttachComponent);
	HaloOfProtectionNiagaraComponent->PassiveSpellTag = FAuraGameplayTags::Get().Abilities_Passive_HaloOfProtection;
	LifeSiphonNiagaraComponent = CreateDefaultSubobject<UPassiveNiagaraComponent>("LifeSiphonComponent");
	LifeSiphonNiagaraComponent->SetupAttachment(EffectAttachComponent);
	LifeSiphonNiagaraComponent->PassiveSpellTag = FAuraGameplayTags::Get().Abilities_Passive_LifeSiphon;
	ManaSiphonNiagaraComponent = CreateDefaultSubobject<UPassiveNiagaraComponent>("ManaSiphonComponent");
	ManaSiphonNiagaraComponent->SetupAttachment(EffectAttachComponent);
	ManaSiphonNiagaraComponent->PassiveSpellTag = FAuraGameplayTags::Get().Abilities_Passive_ManaSiphon;

	CharacterDecal = CreateDefaultSubobject<UDecalComponent>("CharacterDecalComponent");
	CharacterDecal->SetupAttachment(GetRootComponent());
	CharacterDecal->SetVisibleFlag(false);
	CharacterDecal->Deactivate();
}

void AAuraCharacterBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	EffectAttachComponent->SetWorldRotation(FRotator::ZeroRotator);
}

void AAuraCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AAuraCharacterBase, bIsStunned);
	DOREPLIFETIME(AAuraCharacterBase, bIsBurned);
	DOREPLIFETIME(AAuraCharacterBase, bIsBeingShocked);
	DOREPLIFETIME(AAuraCharacterBase, bIsCasting);
	DOREPLIFETIME(AAuraCharacterBase, bIsSlowed);
	DOREPLIFETIME(AAuraCharacterBase, bIsDisarmed);
}

float AAuraCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float DamageTaken = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	OnDamageDelegate.Broadcast(DamageTaken);
	return DamageTaken;
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAnimMontage* AAuraCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

TArray<FTaggedMontage> AAuraCharacterBase::GetAttackMontages_Implementation()
{
	return AttackMontages;
}

UNiagaraSystem* AAuraCharacterBase::GetBloodEffect_Implementation()
{
	return BloodEffect;
}

FTaggedMontage AAuraCharacterBase::GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag)
{
	for (FTaggedMontage TaggedMontage : AttackMontages)
	{
		if (TaggedMontage.MontageTag.MatchesTagExact(MontageTag))
		{
			return TaggedMontage;
		}
	}
	return FTaggedMontage();
}

int32 AAuraCharacterBase::GetMinionCount_Implementation()
{
	return MinionCount;
}

void AAuraCharacterBase::IncrementMinionCount_Implementation(int32 AdditionalMinions)
{
	MinionCount += AdditionalMinions;
}

ECharacterClass AAuraCharacterBase::GetCharacterClass_Implementation()
{
	return CharacterClass;
}

FOnAscRegisteredSignature& AAuraCharacterBase::GetOnAscRegisteredDelegate()
{
	return OnAscRegisteredDelegate;
}

FOnDeathSignature& AAuraCharacterBase::GetOnDeathDelegate()
{
	return OnDeathDelegate;
}

USkeletalMeshComponent* AAuraCharacterBase::GetWeapon_Implementation()
{
	return Weapon;
}

bool AAuraCharacterBase::GetIsBeingShocked_Implementation() const
{
	return bIsBeingShocked;
}

void AAuraCharacterBase::SetIsBeingShocked_Implementation(bool bInShock)
{
	bIsBeingShocked = bInShock;
}

FOnDamageSignature& AAuraCharacterBase::GetOnDamageDelegate()
{
	return OnDamageDelegate;
}

bool AAuraCharacterBase::GetIsCasting_Implementation() const
{
	return bIsCasting;
}

void AAuraCharacterBase::SetIsCasting_Implementation(bool bInCasting)
{
	bIsCasting = bInCasting;
}

bool AAuraCharacterBase::GetIsSlowed_Implementation() const
{
	return bIsSlowed;
}

void AAuraCharacterBase::SetIsSlowed_Implementation(bool bInSlowed)
{
	bIsSlowed = bInSlowed;
}

bool AAuraCharacterBase::GetIsDisarmed_Implementation() const
{
	return bIsDisarmed;
}

void AAuraCharacterBase::SetIsDisarmed_Implementation(bool bInDisarmed)
{
	bIsDisarmed = bInDisarmed;
}

void AAuraCharacterBase::ForceDeath_Implementation()
{
	MulticastHandleDeath(FVector(0.f,0.f,0.f));
}

void AAuraCharacterBase::Die(const FVector& DeathImpulse)
{
	//Drop the weapon
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	MulticastHandleDeath(DeathImpulse);
}

bool AAuraCharacterBase::IsDead_Implementation() const
{
	return bDead;
}

AActor* AAuraCharacterBase::GetAvatar_Implementation()
{
	return this;
}

void AAuraCharacterBase::MulticastHandleDeath_Implementation(const FVector& DeathImpulse)
{
	UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation(), GetActorRotation());
	
	//Things here will de done in server and client
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	Weapon->AddImpulse(DeathImpulse * 0.1f, NAME_None, true);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	GetMesh()->AddImpulse(DeathImpulse, NAME_None, true);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Dissolve();
	bDead = true;
	OnDeathDelegate.Broadcast(this);
}

void AAuraCharacterBase::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bIsStunned = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bIsStunned ? 0.f : BaseWalkSpeed;
}

void AAuraCharacterBase::SlowTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bIsSlowed = NewCount > 0;
	if (bIsSlowed)
	{
		GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed * SlowPercentage;
		GetCharacterMovement()->BrakingDecelerationWalking = OriginalBDWalking * GroundFrictionPercentage;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
		GetCharacterMovement()->BrakingDecelerationWalking = OriginalBDWalking;
	}
}

void AAuraCharacterBase::OnRep_Slowed()
{
}

void AAuraCharacterBase::OnRep_Casting()
{
}

void AAuraCharacterBase::OnRep_Stunned()
{
}

void AAuraCharacterBase::OnRep_Burned()
{
}

void AAuraCharacterBase::OnRep_Disarmed()
{
}

void AAuraCharacterBase::SetCharacterClass(ECharacterClass InClass)
{
	CharacterClass = InClass;
}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	OriginalBDWalking = GetCharacterMovement()->BrakingDecelerationWalking;
}

FVector AAuraCharacterBase::GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag)
{
	const FAuraGameplayTags& AuraGameplayTags = FAuraGameplayTags::Get();
	if (MontageTag.MatchesTagExact(AuraGameplayTags.CombatSocket_Weapon) && IsValid(Weapon))
	{
		return Weapon->GetSocketLocation(WeaponTipSocketName);	
	}
	if (MontageTag.MatchesTagExact(AuraGameplayTags.CombatSocket_LeftHand) && IsValid(GetMesh()))
	{
		return GetMesh()->GetSocketLocation(LeftHandSocketName);	
	}
	if (MontageTag.MatchesTagExact(AuraGameplayTags.CombatSocket_RightHand) && IsValid(GetMesh()))
	{
		return GetMesh()->GetSocketLocation(RightHandSocketName);	
	}
	if (MontageTag.MatchesTagExact(AuraGameplayTags.CombatSocket_Tail) && IsValid(GetMesh()))
	{
		return GetMesh()->GetSocketLocation(TailSocketName);	
	}
	return FVector();
}

void AAuraCharacterBase::InitAbilityActorInfo()
{
}

void AAuraCharacterBase::ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(),GetAbilitySystemComponent());
}

void AAuraCharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultResistanceAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
}

void AAuraCharacterBase::AddCharacterAbilities() const
{
	if (!HasAuthority()) return;
	UAuraAbilitySystemComponent* AuraASC = CastChecked<UAuraAbilitySystemComponent>(GetAbilitySystemComponent());

	AuraASC->AddCharacterAbilities(StartupAbilities);
	AuraASC->AddCharacterPassiveAbilities(StartupPassiveAbilities);
}

void AAuraCharacterBase::Dissolve()
{
	if (IsValid(DissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatIns = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
		GetMesh()->SetMaterial(0, DynamicMatIns);
		StartMeshDissolveTimeline(DynamicMatIns);
	}
	if (IsValid(WeaponDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatIns = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance, this);
		Weapon->SetMaterial(0, DynamicMatIns);
		StartWeaponDissolveTimeline(DynamicMatIns);
	}
}

