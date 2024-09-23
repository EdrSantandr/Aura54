// EdrSantandr Dev


#include "Actor/AuraProjectileBox.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Aura/Aura.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"

AAuraProjectileBox::AAuraProjectileBox()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	SetRootComponent(BoxComponent);
	BoxComponent->SetCollisionObjectType(ECC_PROJECTILE);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	BoxComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	BoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
}

void AAuraProjectileBox::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->OnComponentEndOverlap.AddDynamic(this,&AAuraProjectileBox::OnComponentEndOverlap);
}

void AAuraProjectileBox::Destroyed()
{
	DealDamage();
	Super::Destroyed();
}

void AAuraProjectileBox::DealDamage()
{
	for (AActor* Enemy :CollidedActors)
	{
		if (HasAuthority())
		{
			//The effect should be server only
			if (UAbilitySystemComponent* TargetAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Enemy))
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
				OnHitProjectile(Enemy);
			}
		}
	}
}

void AAuraProjectileBox::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (DamageEffectParams.SourceAbilitySystemComponent == nullptr) return; 
	if (AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor())
	{
		if(SourceAvatarActor == OtherActor) return;
		if (!UAuraAbilitySystemLibrary::IsNotFriend(SourceAvatarActor, OtherActor)) return;
		CollidedActors.AddUnique(OtherActor);
		bHit = true;
	}
}
