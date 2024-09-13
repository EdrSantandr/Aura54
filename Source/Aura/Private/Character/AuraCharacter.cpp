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
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/DeBuff/DeBuffNiagaraComponent.h"
#include "Camera/CameraComponent.h"
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
	AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().DeBuff_Slow, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AAuraCharacter::SlowTagChanged);

	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
		}
		AuraPlayerController->BindConfirmAndCancel();
	}
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
	LoadProgress();
	if (AAuraGameModeBase* AuraGameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		AuraGameModeBase->LoadWorldState(GetWorld());
	}
	/* camera unnatached*/
	TopDownCameraComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
}

void AAuraCharacter::LoadProgress()
{
	if (AAuraGameModeBase* AuraGameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		ULoadScreenSaveGame* SaveData = AuraGameModeBase->RetrieveInGameSaveData();
		if (SaveData == nullptr) return;
		
		/* First Time loading*/
		if (SaveData->bFirstTimeLoadIn)
		{
			InitializeDefaultAttributes();
			AddCharacterAbilities();
		}
		else
		{
			/*Player abilities*/
			if (UAuraAbilitySystemComponent* AuraAsc =Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
			{
				AuraAsc->AddCharacterAbilitiesFromSaveData(SaveData);
			}
			/* Player Stats*/
			if (AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(GetPlayerState()))
			{
				AuraPlayerState->SetLevel(SaveData->PlayerLevel);
				AuraPlayerState->SetXP(SaveData->XP);
				AuraPlayerState->SetAttributePoints(SaveData->AttributePoints);
				AuraPlayerState->SetSpellPoints(SaveData->SpellPoints);
			}
			/*Player Attributes*/
			UAuraAbilitySystemLibrary::InitializeDefaultAttributesFromSaveData(this, AbilitySystemComponent, SaveData);
		}
	}
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

void AAuraCharacter::Die(const FVector& DeathImpulse)
{
	Super::Die(DeathImpulse);

	FTimerDelegate DeathTimerDelegate;
	DeathTimerDelegate.BindLambda([this]()
	{
		if (AAuraGameModeBase* AuraGm = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			AuraGm->LiriaPlayerDied(this);
		}
	});
	GetWorldTimerManager().SetTimer(DeathTimerHandle, DeathTimerDelegate, DeathTime, false);
	TopDownCameraComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
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
		/* First Time Load in*/
		SaveData->bFirstTimeLoadIn = false;
		/* Player Stats */
		if (const AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(GetPlayerState()))
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

		if (!HasAuthority()) return;
		
		/*Saving Abilities*/
		FForEachAbility SaveAbilityDelegate;
		SaveData->SavedAbilities.Empty();
		if (UAuraAbilitySystemComponent* AuraAsc = Cast<UAuraAbilitySystemComponent>(GetAbilitySystemComponent()))
		{
			SaveAbilityDelegate.BindLambda([this,AuraAsc,SaveData](const FGameplayAbilitySpec& AbilitySpec)
			{
				const FGameplayTag AbilityTag = AuraAsc->GetAbilityTagFromSpec(AbilitySpec);
				UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo(this);
				FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
				
				FSavedAbility SavedAbility;
				SavedAbility.GameplayAbility = Info.Ability;
				SavedAbility.AbilityLevel = AbilitySpec.Level;
				SavedAbility.AbilitySlot = AuraAsc->GetSlotInputTagFromAbilityTag(AbilityTag);
				SavedAbility.AbilityStatus = AuraAsc->GetStatusFromAbilityTag(AbilityTag);
				SavedAbility.AbilityTag = AbilityTag;
				SavedAbility.AbilityType = Info.AbilityType;
				
				SaveData->SavedAbilities.AddUnique(SavedAbility);
			});
			AuraAsc->ForEachAbility(SaveAbilityDelegate);
		}
		/* Send the object with data to save*/
		AuraGameModeBase->SaveInGameProgressData(SaveData);
	}
}

void AAuraCharacter::MoveCamera_Implementation(const bool bMoveLeft, const bool bMoveRight, const bool bMoveBottom, const bool bMoveTop, const float CameraSpeed)
{
	FVector MoveVector = FVector(0.f, 0.f, 0.f);
	if (bMoveTop || bMoveBottom)
	{
		if (bMoveTop) MoveVector.X = CameraSpeed;
		if (bMoveBottom) MoveVector.X = -1.0f * CameraSpeed;
		CheckCameraLimits(MaxLocationBottom, MaxLocationTop,TopDownCameraComponent->GetComponentLocation().X, MoveVector.X);
	}
	if (bMoveRight || bMoveLeft)
	{
		if (bMoveRight) MoveVector.Y = CameraSpeed;
		if (bMoveLeft) MoveVector.Y = -1.0f * CameraSpeed;
		CheckCameraLimits(MaxLocationLeft, MaxLocationRight, TopDownCameraComponent->GetComponentLocation().Y,MoveVector.Y);
	}
	TopDownCameraComponent->AddRelativeLocation(MoveVector);
}

void AAuraCharacter::CheckCameraLimits(float const LowerLimit, float const UpperLimit, float const CameraComponentLocation, double& CameraMovement)
{
	const double NewPosition = CameraComponentLocation + CameraMovement;
	if (NewPosition<LowerLimit || UpperLimit<NewPosition)
		CameraMovement = 0;
}

void AAuraCharacter::FocusCharacterCamera_Implementation()
{
	TopDownCameraComponent->SetRelativeLocation(SpringArmComponent->GetSocketLocation(USpringArmComponent::SocketName));
}
