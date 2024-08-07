// EdrSantander Dev


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "NiagaraComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/DeBuff/DeBuffNiagaraComponent.h"
#include "Camera/CameraComponent.h"
#include "Game/AuraGameInstance.h"
#include "Game/AuraGameModeBase.h"
#include "Game/LoadScreenSaveGame.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->SetUsingAbsoluteRotation(true);
	SpringArmComponent->bDoCollisionTest = false;
	
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>("TopDownCameraComponent");
	TopDownCameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false;
	
	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("LevelUpNiagaraComponent");
	LevelUpNiagaraComponent->SetupAttachment(GetRootComponent());
	LevelUpNiagaraComponent->bAutoActivate = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f,400.f,0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	CharacterClass = ECharacterClass::Elementalist;
}

void AAuraCharacter::OnRep_Stunned()
{
	if (UAuraAbilitySystemComponent* AuraAsc = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		const FAuraGameplayTags& AuraGameplayTags = FAuraGameplayTags::Get();
		FGameplayTagContainer BlockedTags;
		BlockedTags.AddTag(AuraGameplayTags.Player_Block_InputReleased);
		BlockedTags.AddTag(AuraGameplayTags.Player_Block_CursorTrace);
		BlockedTags.AddTag(AuraGameplayTags.Player_Block_InputHeld);
		BlockedTags.AddTag(AuraGameplayTags.Player_Block_InputPressed);
		if (bIsStunned)
		{
			AuraAsc->AddLooseGameplayTags(BlockedTags);
			StunDeBuffComponent->Activate();
		}
		else
		{
			AuraAsc->RemoveLooseGameplayTags(BlockedTags);
			StunDeBuffComponent->Deactivate();
		}
	}
}

void AAuraCharacter::OnRep_Burned()
{
	if (bIsBurned)
	{
		BurnDeBuffComponent->Activate();
	}
	else
	{
		BurnDeBuffComponent->Deactivate();
	}
}

void AAuraCharacter::InitAbilityActorInfo()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState,this);
	Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet = AuraPlayerState->GetAttributeSet();
	OnAscRegisteredDelegate.Broadcast(AbilitySystemComponent);

	AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().DeBuff_Stun, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AAuraCharacter::StunTagChanged);

	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
	InitializeDefaultAttributes();
}

void AAuraCharacter::MulticastLevelUpParticles_Implementation() const
{
	if (IsValid(LevelUpNiagaraComponent))
	{
		const FVector CameraLocation = TopDownCameraComponent->GetComponentLocation();
		const FVector NiagaraSystemLocation = LevelUpNiagaraComponent->GetComponentLocation();
		const FRotator ToCameraRotation = (CameraLocation-NiagaraSystemLocation).Rotation();
		LevelUpNiagaraComponent->SetWorldRotation(ToCameraRotation);
		LevelUpNiagaraComponent->Activate(true);
	}
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
	MulticastLevelUpParticles();
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
		AuraPlayerState->AddToLevel(InPlayerLevel);

		if (UAuraAbilitySystemComponent* AuraAsc = Cast<UAuraAbilitySystemComponent>(GetAbilitySystemComponent()))
		{
			AuraAsc->UpdateAbilitiesStatuses(AuraPlayerState->GetPlayerLevel());
		}
	}
}

void AAuraCharacter::AddToSpellPoints_Implementation(int32 InSpellPoints)
{
	if (AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(GetPlayerState()))
	{
		AuraPlayerState->AddToSpellPoints(InSpellPoints);
	}
}

void AAuraCharacter::AddToAttributePoints_Implementation(int32 InAttributePoints)
{
	if (AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(GetPlayerState()))
	{
		AuraPlayerState->AddToAttributePoints(InAttributePoints);
	}
}

int32 AAuraCharacter::GetAttributePoints_Implementation() const
{
	if (const AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(GetPlayerState()))
	{
		return AuraPlayerState->GetAttributePoints();
	}
	return 0;
}

int32 AAuraCharacter::GetSpellPoints_Implementation() const
{
	if (const AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(GetPlayerState()))
	{
		return AuraPlayerState->GetSpellPoints();
	}
	return 0;
}

void AAuraCharacter::ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial)
{
	if (AAuraPlayerController* AuraPC = Cast<AAuraPlayerController>(GetController()))
	{
		AuraPC->ShowMagicCircle(DecalMaterial);
		AuraPC->bShowMouseCursor = false;
	}
}

void AAuraCharacter::HideMagicCircle_Implementation()
{
	if (AAuraPlayerController* AuraPC = Cast<AAuraPlayerController>(GetController()))
	{
		AuraPC->HideMagicCircle();
		AuraPC->bShowMouseCursor = true;
	}	
}

void AAuraCharacter::SaveProgress_Implementation(const FName& CheckPointTag)
{
	if (AAuraGameModeBase* AuraGameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		ULoadScreenSaveGame* SaveData = AuraGameModeBase->RetrieveInGameSaveData();
		if (SaveData == nullptr) return;
		/* Checkpoint tag*/
		SaveData->PlayerStartTag = CheckPointTag;
		/* Player Stats */
		if (AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(GetPlayerState()))
		{
			SaveData->PlayerLevel = AuraPlayerState->GetPlayerLevel();
			SaveData->XP = AuraPlayerState->GetXP();
			SaveData->AttributePoints = AuraPlayerState->GetAttributePoints();
			SaveData->SpellPoints = AuraPlayerState->GetSpellPoints();
		}
		/* Player Attributes*/
		SaveData->Intelligence = UAuraAttributeSet::GetIntelligenceAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Strength = UAuraAttributeSet::GetStrengthAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Resilience = UAuraAttributeSet::GetResilienceAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Vigor = UAuraAttributeSet::GetVigorAttribute().GetNumericValue(GetAttributeSet());
		/* Send the object with data to save*/
		AuraGameModeBase->SaveInGameProgressData(SaveData);
	}
}
