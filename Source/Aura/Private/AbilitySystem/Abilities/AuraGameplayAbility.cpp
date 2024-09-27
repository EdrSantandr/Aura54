// EdrSantander Dev


#include "AbilitySystem/Abilities/AuraGameplayAbility.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/MessageInfo.h"
#include "Game/AuraGameModeBase.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

void UAuraGameplayAbility::SendMessage(const FGameplayTag& InMessageTag) const
{
	if (const AAuraGameModeBase * AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		if (const UPlayerMessageInfo* PlayerMessageInfo = UAuraAbilitySystemLibrary::GetPlayerMessageInfo(GetWorld()))
		{
			const FMessageInfo MessageForPlayer = PlayerMessageInfo->FindMessagePlayerInfoByTag(InMessageTag);
			AuraGameMode->OnAbilityPlayerMessageSignature.Broadcast(MessageForPlayer.MessageContent.ToString());
		}
	}
}

void UAuraGameplayAbility::CheckCostAndCooldown(const FGameplayAbilityActorInfo InActorInfo) const
{
	if (bCheckCooldown||bCheckCost)
	{
		if (bCheckCooldown)
		{
			if (!FMath::IsNearlyZero(GetCooldownTimeRemaining()))
			{
				if (!CheckCooldown(CurrentSpecHandle, &InActorInfo))
				{
					SendMessage(FAuraGameplayTags::Get().Message_NotReady);
				}
			}
		}
		if (bCheckCost)
		{
			if (!CheckCost(CurrentSpecHandle, &InActorInfo))
			{
				SendMessage(FAuraGameplayTags::Get().Message_NotEnoughMana);
			}
		}	
	}
}

void UAuraGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	SaveCharacterWalkSpeed(ActorInfo);
}

void UAuraGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	SetCharacterWalkSpeed(ActorInfo);
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UAuraGameplayAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                              const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                              const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	CheckCostAndCooldown(*ActorInfo);
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

FString UAuraGameplayAbility::GetDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>%s, </><Level>%d</>"),L"Default Ability Name - Lorem ipsum Lorem ipsum Lorem ipsum Lorem ipsum Lorem ipsum Lorem ipsum Lorem ipsum Lorem ipsum Lorem ipsum Lorem ipsum Lorem ipsum Lorem ipsum Lorem ipsum Lorem ipsum Lorem ipsum",Level);
}

FString UAuraGameplayAbility::GetNextLevelDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>Next Level </><Level>%d</> \n<Default>Causes more damage</>"),Level);
}

FString UAuraGameplayAbility::GetLockedDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>Spell locked until level:</><Level>%d</>"), Level);
}

float UAuraGameplayAbility::GetManaCost(float InLevel) const
{
	float ManaCost = 0.f;
	if (const UGameplayEffect* CostEffect = GetCostGameplayEffect())
	{
		for(auto Mod : CostEffect->Modifiers)
		{
			if (Mod.Attribute == UAuraAttributeSet::GetManaAttribute())
			{
				Mod.ModifierMagnitude.GetStaticMagnitudeIfPossible(InLevel,ManaCost);
				break;
			}
		}
	}
	return ManaCost;
}

float UAuraGameplayAbility::GetCooldown(float InLevel) const
{
	float Cooldown = 0.f;
	if (const UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect())
	{
		CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(InLevel, Cooldown);
	}
	return Cooldown;
}

void UAuraGameplayAbility::SaveCharacterWalkSpeed(const FGameplayAbilityActorInfo* ActorInfo)
{
	if (const ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor);!FMath::IsNearlyZero(Character->GetCharacterMovement()->MaxWalkSpeed))
	{
		CharacterWalkSpeed = Character->GetCharacterMovement()->MaxWalkSpeed;
	}
}

void UAuraGameplayAbility::SetCharacterWalkSpeed(const FGameplayAbilityActorInfo* ActorInfo) const
{
	if (const ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor); !FMath::IsNearlyZero(CharacterWalkSpeed))
	{
		if (IsValid(Character))
		{
			Character->GetCharacterMovement()->MaxWalkSpeed = CharacterWalkSpeed;	
		}
	}
}

float UAuraGameplayAbility::GetCurrentLevelManaCost() const
{
	float ManaCost = 0.f;
	if (const UGameplayEffect* CostEffect = GetCostGameplayEffect())
	{
		for(auto Mod : CostEffect->Modifiers)
		{
			if (Mod.Attribute == UAuraAttributeSet::GetManaAttribute())
			{
				Mod.ModifierMagnitude.GetStaticMagnitudeIfPossible(GetAbilityLevel(),ManaCost);
				break;
			}
		}
	}
	return ManaCost;
}

void UAuraGameplayAbility::SendMessageBP(const FGameplayTag& InMessageTag)
{
	SendMessage(InMessageTag);
}
