// EdrSantander Dev


#include "AbilitySystem/Abilities/AuraGameplayAbility.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/MessageInfo.h"

void UAuraGameplayAbility::SendMessage(const FGameplayTag& InMessageTag)
{
	if (const UAuraAbilitySystemComponent* AuraAsc = Cast<UAuraAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo()))
	{
		if (const UPlayerMessageInfo* PlayerMessageInfo = UAuraAbilitySystemLibrary::GetPlayerMessageInfo(GetWorld()))
		{
			const FMessageInfo MessageForPlayer = PlayerMessageInfo->FindMessagePlayerInfoByTag(InMessageTag);
			AuraAsc->OnAbilityPlayerMessageSignature.Broadcast(MessageForPlayer.MessageContent.ToString());	
		}
	}
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
