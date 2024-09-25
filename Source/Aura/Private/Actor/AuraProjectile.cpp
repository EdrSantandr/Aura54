// EdrSantander Dev


#include "Actor/AuraProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Aura/Aura.h"
#include "Components/AudioComponent.h"
#include "Interaction/CombatInterface.h"

AAuraProjectile::AAuraProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true; // Doing this so the server spawns and the clients just see a copy
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SetRootComponent(SphereComponent);
	SphereComponent->SetCollisionObjectType(ECC_PROJECTILE);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileComponent");
	ProjectileMovementComponent->InitialSpeed = 550.f;
	ProjectileMovementComponent->MaxSpeed = 550.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
}

void AAuraProjectile::BindTargetDestroy(AActor* HomingTarget)
{
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(HomingTarget))
		CombatInterface->GetOnDeathDelegate().AddDynamic(this, &AAuraProjectile::OnTargetDie);
}

void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();
	if (bBindRootOnOverlap)
		SphereComponent->OnComponentBeginOverlap.AddDynamic(this,&AAuraProjectile::OnBeginOverlap);
	SetLifeSpan(LifeSpan);
	SetReplicateMovement(true);
	LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent());
}

void AAuraProjectile::OnHit()
{
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}
	bHit = true;
}

void AAuraProjectile::Destroyed()
{
	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}
	if (!bHit && !HasAuthority()) OnHit();
	Super::Destroyed();
}

void AAuraProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (DamageEffectParams.SourceAbilitySystemComponent == nullptr) return; 
	if (AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor())
	{
		if(SourceAvatarActor == OtherActor) return;
		if (!UAuraAbilitySystemLibrary::IsNotFriend(SourceAvatarActor, OtherActor)) return;
		if(!bHit) OnHit();
		
		if (HasAuthority())
		{
			//The effect should be server only
			if (UAbilitySystemComponent* TargetAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
			{
				const FVector DeathImpulseVector = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
				DamageEffectParams.DeathImpulse = DeathImpulseVector;
				const bool bKnockBack = FMath::RandRange(1,100) < DamageEffectParams.KnockBackChance;
				if (bKnockBack)
				{
					FRotator Rotation = GetActorRotation();
					Rotation.Pitch = 45.f;
				
					const FVector KnockBackDirection = Rotation.Vector();
					const FVector KnockBackForce = KnockBackDirection * DamageEffectParams.KnockBackForceMagnitude;
					DamageEffectParams.KnockBackForce = KnockBackForce;
				}
			
				DamageEffectParams.TargetAbilitySystemComponent = TargetAsc;
				UAuraAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
				OnHitProjectile(OtherActor);
			}
			Destroy();
		}
		else
		{
			bHit = true;
		}
	}
}

void AAuraProjectile::AreaRadialDamage(bool bOverRideRadius, float RadiusOverwritten)
{
	if (DamageEffectParams.SourceAbilitySystemComponent == nullptr) return; 
	if (AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor())
	{
		TArray<AActor*> Enemies;
		const TArray<AActor*> ActorsToIgnore;
		const float Radius = bOverRideRadius? RadiusOverwritten:SphereComponent->GetScaledSphereRadius();  
		UAuraAbilitySystemLibrary::GetLiveEnemiesWithinRadius(GetWorld(), Enemies,ActorsToIgnore, Radius, GetActorLocation());
		if (Enemies.Num()>0)
		{
			for (AActor* Enemy : Enemies)
			{
				if (UAbilitySystemComponent* TargetAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Enemy))
				{
					DamageEffectParams.TargetAbilitySystemComponent = TargetAsc;
					UAuraAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
					OnHitProjectile(Enemy);
				}
			}
		}
		if(!bHit) OnHit();
		if (HasAuthority())
		{
			Destroy();
		}
		else
		{
			bHit = true;
		}
	}
}

void AAuraProjectile::OnTargetDie(AActor* DeadActor)
{
	ProjectileMovementComponent->StopMovementImmediately();
	OnHit();
	Destroy();
}
