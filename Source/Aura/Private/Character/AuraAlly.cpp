// EdrSantandr Dev


#include "Character/AuraAlly.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AI/AuraAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/AuraEnemy.h"
#include "Components/CapsuleComponent.h"
#include "Components/DecalComponent.h"
#include "Components/WidgetComponent.h"
#include "Game/AuraGameModeBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Widget/AuraUserWidget.h"

AAuraAlly::AAuraAlly()
{
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(false);

	BeamOriginComponent = CreateDefaultSubobject<UStaticMeshComponent>("BeamOriginComponent");
	BeamOriginComponent->SetupAttachment(GetRootComponent());
	BeamOriginComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	BeamOriginComponent->SetHiddenInGame(true);
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Block);
	
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
	
	BaseWalkSpeed = 0; // Trees won't move
}

void AAuraAlly::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!HasAuthority()) return; // This should only run on the server

	GetMesh()->SetCustomDepthStencilValue(StencilColorValue);
	AuraAIController = Cast<AAuraAIController>(NewController);
	AuraAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"),false);
	const bool RangedAttacker = CharacterClass != ECharacterClass::Warrior;
	AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"), RangedAttacker);
	if (bIsGate && MainGoal)
	{
		//Execute init ability for gate
		CreatePathsFromGate(3, GetActorLocation(), MainGoal->GetActorLocation());
		FVector Direction = MainGoal->GetActorLocation() - GetActorLocation();
		Direction.Normalize();
		SpawnLocations = UAuraAbilitySystemLibrary::EvenlySpreadVectors(Direction, FVector::ZAxisVector, AngleSpread, SpawnApertures);
		AuraAIController->RunBehaviorTree(BehaviorTree);
	}
}

ECharacterClass AAuraAlly::GetCharacterClass_Implementation()
{
	return CharacterClass;
}

AActor* AAuraAlly::GetCombatTargetAlly_Implementation() const
{
	return CombatTarget;
}

void AAuraAlly::SetCombatTargetAlly_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

void AAuraAlly::SetLevel_Implementation(const int32 NewLevel)
{
	Level = NewLevel;
	if (UAuraAbilitySystemComponent* AuraAsc = Cast<UAuraAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		AuraAsc->UpdateAbilitiesOfCharacter();
	}
}

bool AAuraAlly::GetIsGate_Implementation()
{
	return bIsGate;
}

void AAuraAlly::HighlightActor_Implementation()
{
	GetMesh()->SetRenderCustomDepth(true);
}

void AAuraAlly::UnHighlightActor_Implementation()
{
	GetMesh()->SetRenderCustomDepth(false);
}

void AAuraAlly::SetMoveToLocation_Implementation(FVector& OutDestination)
{
	// DO nothing
}

void AAuraAlly::ShowDecal() const
{
	if (IsValid(CharacterDecal->GetDecalMaterial()))
	{
		CharacterDecal->SetVisibility(true);
		CharacterDecal->Activate();	
	}
}

void AAuraAlly::HideDecal() const
{
	if (IsValid(CharacterDecal->GetDecalMaterial()))
	{
		CharacterDecal->SetVisibility(false);
		CharacterDecal->Deactivate();
	}
}

int32 AAuraAlly::GetPlayerLevel_Implementation()
{
	return Level;
}

void AAuraAlly::Die(const FVector& DeathImpulse)
{
	if (bIsGate)
	{
		SpawnSentinel();
		SetLifeSpan(1.f);
		//Remove from the auragamemodebase
		if (AAuraGameModeBase* AuraGameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			AuraGameModeBase->RemoveGate(this);
		}
	}
	else
	{
		SetLifeSpan(Lifespan);
	}
	//REMOVE ACTORS
	if (VisualEffectActor && IsValid(VisualEffectActor)) VisualEffectActor->Destroy();
	if (AuraAIController) AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("Dead"), true);
	Super::Die(FVector::ZeroVector);
}

UMeshComponent* AAuraAlly::GetBeamOriginComponent_Implementation()
{
	return BeamOriginComponent;
}

void AAuraAlly::MulticastHandleDeath_Implementation(const FVector& DeathImpulse)
{
	UGameplayStatics::PlaySound2D(this, DeathSound);
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	Weapon->AddImpulse(DeathImpulse * 0.1f, NAME_None, true);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Dissolve();
	bDead = true;
	OnDeathDelegate.Broadcast(this);
}

void AAuraAlly::Dissolve()
{
	if (IsValid(DissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatIns = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
		GetMesh()->SetMaterial(0, DynamicMatIns);
		StartMeshDissolveTimeline(DynamicMatIns);
	}
}

void AAuraAlly::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
	if (AuraAIController && AuraAIController->GetBlackboardComponent())
	{
		AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"),bHitReacting);	
	}
}

void AAuraAlly::SetDecalMaterial(UMaterialInterface* InDecalMaterial) const
{
	CharacterDecal->SetMaterial(0, InDecalMaterial);
}

void AAuraAlly::SpawnSentinel() const
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn; 
	AAuraAlly* Sentinel = GetWorld()->SpawnActorDeferred<AAuraAlly>(SentinelSpawnClass, GetTransform());
	//Sentinel->SetCharacterClass(SentinelClass);
	Sentinel->FinishSpawning(GetTransform());
	Sentinel->SpawnVisualEffectActor();
	Sentinel->SpawnDefaultController();
}

void AAuraAlly::SetCharacterClass(ECharacterClass InClass)
{
	Super::SetCharacterClass(InClass);
	AuraAIController->RunBehaviorTree(BehaviorTree);
}

void AAuraAlly::ActivateBehaviorTree(AAIController* InAiController) const
{
	InAiController->RunBehaviorTree(BehaviorTree);
}

void AAuraAlly::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	InitAbilityActorInfo();
	if(HasAuthority())
	{
		UAuraAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent, CharacterClass);
	}
	
	if (UAuraUserWidget* AuraUserWidget = Cast<UAuraUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		AuraUserWidget->SetWidgetController(this);
	}
	
	if (const UAuraAttributeSet* AuraAs = Cast<UAuraAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAs->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAs->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);
		FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
		AbilitySystemComponent->RegisterGameplayTagEvent(GameplayTags.Effects_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(
			this,
			&AAuraAlly::HitReactTagChanged
		);
		
		OnHealthChanged.Broadcast(AuraAs->GetHealth());
		OnMaxHealthChanged.Broadcast(AuraAs->GetMaxHealth());
	}
	
}

void AAuraAlly::SpawnVisualEffectActor()
{
	if (VisualEffectActor && IsValid(VisualEffectActor))
	{
		VisualEffectActor->Destroy();
		VisualEffectActor = nullptr;
	}
	if (VisualEffectActor == nullptr)
	{
		if (const TSubclassOf<AActor> VisualEffectClass = UAuraAbilitySystemLibrary::GetVisualEffectByCharacterClass(this, CharacterClass))
		{
			FTransform SpawnTransform;
			FVector EffectSpawnLocation = GetActorLocation();
			EffectSpawnLocation += FVector(0.f,0.f,EffectHeight);
			SpawnTransform.SetLocation(EffectSpawnLocation);
			SpawnTransform.SetRotation(GetActorRotation().Quaternion());
			VisualEffectActor = GetWorld()->SpawnActorDeferred<AActor>(VisualEffectClass, SpawnTransform, this, Cast<APawn>(this), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
			VisualEffectActor->SetOwner(this);
			VisualEffectActor->FinishSpawning(SpawnTransform);
		}	
	}
}

void AAuraAlly::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().DeBuff_Stun, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AAuraAlly::StunTagChanged);
	if (HasAuthority())
	{
		InitializeDefaultAttributes();	
	}
	OnAscRegisteredDelegate.Broadcast(AbilitySystemComponent);
	// Get the Decal material
	if (UMaterialInterface* DecalMaterial = UAuraAbilitySystemLibrary::GetMaterialDecalByCharacterClass(this, CharacterClass))
	{
		CharacterDecal->SetMaterial(0, DecalMaterial);	
	}
	// Get the VisualEffect
	SpawnVisualEffectActor();
}

void AAuraAlly::InitializeDefaultAttributes() const
{
	UAuraAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}

void AAuraAlly::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	Super::StunTagChanged(CallbackTag, NewCount);
	if (AuraAIController && AuraAIController->GetBlackboardComponent())
	{
		AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("Stunned"),bIsStunned);	
	}
}

void AAuraAlly::Destroyed()
{
	Super::Destroyed();
	if (VisualEffectActor)
		VisualEffectActor->Destroy();
}

void AAuraAlly::CreatePathsFromGate(const int32 NumPaths, const FVector& InOriginalPoint, const FVector& InFinalPoint)
{
	TArray<AActor*> ActorsFound;
	TArray<AActor*> ActorsToIgnore; //todo: add actors to ignore
	const float XPathDimension = (InFinalPoint - InOriginalPoint).Length()*0.5f - 25.f; //This 5.f is just to don't consider Initial and FinalPoints
	UAuraAbilitySystemLibrary::GetPathPointsInsideBox(GetWorld(), ActorsFound, ActorsToIgnore, InOriginalPoint, InFinalPoint, XPathDimension, YPathDimension);
	Algo::Sort(ActorsFound, FSortVectorByLenght(InOriginalPoint));
	for (int32 i=0;i<NumPaths; i++)
	{
		TArray<FVector> PathToGoal = CreateSinglePath(ActorsFound, InOriginalPoint, InFinalPoint);
		PathsByPoint.Add(PathToGoal);
	}
}

TArray<FVector> AAuraAlly::CreateSinglePath(const TArray<AActor*>& InActors, const FVector& InOriginalPoint,
	const FVector& InFinalPoint)
{
	TArray<FVector> Result;
	TArray<AActor*> FoundActors = InActors;
	FVector const Origin = InOriginalPoint;
	FVector const Final = InFinalPoint;
	Result.Add(Origin);
	for (int32 Index=0;Index<FoundActors.Num();Index++)
	{
		if (FMath::FRandRange(0.f,1.f) < 0.7f) // this 0.7f just to make the path more consistent
			{
			Result.Add(FoundActors[Index]->GetActorLocation());
			}
	}
	Result.Add(Final);
	return Result;
}
