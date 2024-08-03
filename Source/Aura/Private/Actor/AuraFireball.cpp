// EdrSantandr Dev


#include "Actor/AuraFireball.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"

void AAuraFireball::BeginPlay()
{
	Super::BeginPlay();
	StartOutgoingTimeline();
}

void AAuraFireball::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (DamageEffectParams.SourceAbilitySystemComponent == nullptr) return; 
	if (AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor())
	{
		if(SourceAvatarActor == OtherActor) return;
		if (!UAuraAbilitySystemLibrary::IsNotFriend(SourceAvatarActor, OtherActor)) return;

		if (HasAuthority())
		{
			if (UAbilitySystemComponent* TargetAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
			{
				const FVector DeathImpulseVector = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
				DamageEffectParams.DeathImpulse = DeathImpulseVector;
				DamageEffectParams.TargetAbilitySystemComponent = TargetAsc;
				UAuraAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
			}
		}
	}
}
