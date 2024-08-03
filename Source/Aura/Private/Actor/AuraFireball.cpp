// EdrSantandr Dev


#include "Actor/AuraFireball.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "GameplayCueManager.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Components/AudioComponent.h"

void AAuraFireball::BeginPlay()
{
	Super::BeginPlay();
	StartOutgoingTimeline();
}

void AAuraFireball::OnHit()
{
	//execute gameplay cue locally
	if (GetOwner())
	{
		FGameplayCueParameters CueParameters;
		CueParameters.Location = GetActorLocation();
		UGameplayCueManager::ExecuteGameplayCue_NonReplicated(GetOwner(), FAuraGameplayTags::Get().GameplayCue_FireBlast, CueParameters);
	}
	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}
	bHit = true;
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
