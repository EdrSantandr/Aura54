// EdrSantander Dev


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f,400.f,0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	CharacterClass = ECharacterClass::Elementalist;
}

void AAuraCharacter::InitAbilityActorInfo()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState,this);
	Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet = AuraPlayerState->GetAttributeSet();

	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
	InitializeDefaultAttributes();
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	//init ability actor info for the server
	InitAbilityActorInfo();
	AddCharacterAbilities();
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	//init ability actor info for the client
	InitAbilityActorInfo();
}

int32 AAuraCharacter::GetPlayerLevel_Implementation()
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetPlayerLevel();
}

void AAuraCharacter::AddToExp_Implementation(int32 InExp)
{
	if (AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(GetPlayerState()))
	{
		AuraPlayerState->AddToXP(InExp);
	}
}

void AAuraCharacter::LevelUp_Implementation()
{
	
}

int32 AAuraCharacter::GetExp_Implementation() const
{
	if (const AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(GetPlayerState()))
	{
		return AuraPlayerState->GetXP();
	}
	return 0;
}

int32 AAuraCharacter::FindLevelForExp_Implementation(int32 InExp) const
{
	if (const AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(GetPlayerState()))
	{
		return AuraPlayerState->LevelUpInfo->FindLevelForExp(InExp);
	}
	return 0;
}

int32 AAuraCharacter::GetAttributePointsReward_Implementation(int32 Level) const
{
	if (const AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(GetPlayerState()))
	{
		return AuraPlayerState->LevelUpInfo->LevelUpInformation[Level].AttributePointAward;
	}
	return 0;
}

int32 AAuraCharacter::GetSpellPointsReward_Implementation(int32 Level) const
{
	if (const AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(GetPlayerState()))
	{
		return AuraPlayerState->LevelUpInfo->LevelUpInformation[Level].SpellPointAward;
	}
	return 0;
}

void AAuraCharacter::AddToPlayerLevel_Implementation(int32 InPlayerLevel)
{
	if (AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(GetPlayerState()))
	{
		return AuraPlayerState->AddToLevel(InPlayerLevel);
	}
}

void AAuraCharacter::AddToSpellPoints_Implementation(int32 InSpellPoints)
{
	//todo: add attribute points to player state
}

void AAuraCharacter::AddToAttributePoints_Implementation(int32 InAttributePoints)
{
	//todo: add spell points to player state
}
