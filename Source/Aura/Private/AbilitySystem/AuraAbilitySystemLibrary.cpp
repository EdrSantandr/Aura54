// EdrSantander Dev


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "Game/AuraGameModeBase.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"

bool UAuraAbilitySystemLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, AAuraHUD*& OutAuraHud)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject,0))
	{
		OutAuraHud = Cast<AAuraHUD>(PC->GetHUD());
		if (OutAuraHud)
		{
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			OutWCParams.AttributeSet = AS;
			OutWCParams.PlayerController = PC;
			OutWCParams.PlayerState = PS;
			OutWCParams.AbilitySystemComponent = ASC;
			return true;
		}
	}
	return false;
}

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WcParams;
	AAuraHUD* AuraHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WcParams, AuraHUD))
	{
		return AuraHUD->GetOverlayWidgetController(WcParams);
	}
	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WcParams;
	AAuraHUD* AuraHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WcParams, AuraHUD))
	{
		return AuraHUD->GetAttributeMenuWidgetController(WcParams);
	}
	return nullptr;
}

USpellMenuWidgetController* UAuraAbilitySystemLibrary::GetSpellMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WcParams;
	AAuraHUD* AuraHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WcParams, AuraHUD))
	{
		return AuraHUD->GetSpellMenuWidgetController(WcParams);
	}
	return nullptr;
}

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* Asc)
{
	const AActor* AvatarActor = Asc->GetAvatarActor();
	
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	const FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

	FGameplayEffectContextHandle PrimaryEffectContextHandle = Asc->MakeEffectContext();
	PrimaryEffectContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = Asc->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level,PrimaryEffectContextHandle);
	Asc->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle SecondaryEffectContextHandle = Asc->MakeEffectContext();
	SecondaryEffectContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = Asc->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level, SecondaryEffectContextHandle);
	Asc->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle ResistanceEffectContextHandle = Asc->MakeEffectContext();
	ResistanceEffectContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle ResistanceAttributesSpecHandle = Asc->MakeOutgoingSpec(CharacterClassInfo->ResistanceAttributes, Level, ResistanceEffectContextHandle);
	Asc->ApplyGameplayEffectSpecToSelf(*ResistanceAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalEffectContextHandle = Asc->MakeEffectContext();
	VitalEffectContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = Asc->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, VitalEffectContextHandle);
	Asc->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle .Data.Get());
}

void UAuraAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* Asc, ECharacterClass CharacterClass)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return;
	for (const TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		Asc->GiveAbility(AbilitySpec);
	}
	const FCharacterClassDefaultInfo DefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.CharacterClassAbilities)
	{
		if (Asc->GetAvatarActor()->Implements<UCombatInterface>())
		{
			const int32 PlayerLevel = ICombatInterface::Execute_GetPlayerLevel(Asc->GetAvatarActor());
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, PlayerLevel);
			Asc->GiveAbility(AbilitySpec);
		}
	}
}

UCharacterClassInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	const AAuraGameModeBase* AuraGameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (AuraGameModeBase == nullptr) return nullptr;
	return AuraGameModeBase->CharacterClassInfo;
}

UAbilityInfo* UAuraAbilitySystemLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	const AAuraGameModeBase* AuraGameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (AuraGameModeBase == nullptr) return nullptr;
	return AuraGameModeBase->AbilityInfo;
}

bool UAuraAbilitySystemLibrary::IsSuccessfulDeBuff(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraGameplayEffectContext->IsSuccessfulDeBuff();
	}
	return false;
}

float UAuraAbilitySystemLibrary::GetDeBuffDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraGameplayEffectContext->GetDeBuffDamage();
	}
	return 0.f;
}

float UAuraAbilitySystemLibrary::GetDeBuffDuration(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraGameplayEffectContext->GetDeBuffDuration();
	}
	return 0.f;
}

float UAuraAbilitySystemLibrary::GetDeBuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraGameplayEffectContext->GetDeBuffFrequency();
	}
	return 0.f;
}

FGameplayTag UAuraAbilitySystemLibrary::GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		if (AuraGameplayEffectContext->GetDamageType().IsValid())
		{
			return *AuraGameplayEffectContext->GetDamageType();
		}
	}
	return FGameplayTag();
}

FVector UAuraAbilitySystemLibrary::GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraGameplayEffectContext->GetDeathImpulse();
	}
	return FVector::ZeroVector;
}

FVector UAuraAbilitySystemLibrary::GetKnockBackForce(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraGameplayEffectContext->GetKnockBackForce();
	}
	return FVector::ZeroVector;
}

int32 UAuraAbilitySystemLibrary::GetXpRewardForClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 CharacterLevel)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return 0;

	const FCharacterClassDefaultInfo Info = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	const float XpReward = Info.XPReward.GetValueAtLevel(CharacterLevel);
	
	return static_cast<int32>(XpReward);
}


bool UAuraAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraGameplayEffectContext->IsBlockedHit();
	}
	return false;
}

bool UAuraAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraGameplayEffectContext->IsCriticalHit();
	}
	return false;
}

bool UAuraAbilitySystemLibrary::IsRadialDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraGameplayEffectContext->IsRadialDamage();
	}
	return false;
}

float UAuraAbilitySystemLibrary::GetRadialDamageInnerRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraGameplayEffectContext->GetRadialDamageInnerRadius();
	}
	return 0.f;
}

float UAuraAbilitySystemLibrary::GetRadialDamageOuterRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraGameplayEffectContext->GetRadialDamageOuterRadius();
	}
	return 0.f;
}

FVector UAuraAbilitySystemLibrary::GetRadialDamageOrigin(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraGameplayEffectContext->GetRadialDamageOrigin();
	}
	return FVector::ZeroVector;
}

void UAuraAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit)
{
	if (FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast< FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraGameplayEffectContext->SetIsBlockedHit(bInIsBlockedHit);
	}
}

void UAuraAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit)
{
	if (FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast< FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraGameplayEffectContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}

void UAuraAbilitySystemLibrary::SetIsSuccessfulDeBuff(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsSuccessfulDeBuff)
{
	if (FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast< FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraGameplayEffectContext->SetIsSuccessfulDeBuff(bInIsSuccessfulDeBuff);
	}
}

void UAuraAbilitySystemLibrary::SetDeBuffDamage(FGameplayEffectContextHandle& EffectContextHandle, float InDeBuffDamage)
{
	if (FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast< FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraGameplayEffectContext->SetDeBuffDamage(InDeBuffDamage);
	}
}

void UAuraAbilitySystemLibrary::SetDeBuffDuration(FGameplayEffectContextHandle& EffectContextHandle, float InDeBuffDuration)
{
	if (FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast< FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraGameplayEffectContext->SetDeBuffDuration(InDeBuffDuration);
	}
}

void UAuraAbilitySystemLibrary::SetDeBuffFrequency(FGameplayEffectContextHandle& EffectContextHandle, float InDeBuffFrequency)
{
	if (FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast< FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraGameplayEffectContext->SetDeBuffFrequency(InDeBuffFrequency);
	}
}

void UAuraAbilitySystemLibrary::SetDamageType(FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InDamageType)
{
	if (FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast< FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		const TSharedPtr<FGameplayTag> DamageType = MakeShared<FGameplayTag>(InDamageType);
		AuraGameplayEffectContext->SetDamageType(DamageType);
	}
}

void UAuraAbilitySystemLibrary::SetDeathImpulse(FGameplayEffectContextHandle& EffectContextHandle, const FVector& InDeathImpulse)
{
	if (FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast< FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraGameplayEffectContext->SetDeathImpulse(InDeathImpulse);
	}
}

void UAuraAbilitySystemLibrary::SetKnockBackForce(FGameplayEffectContextHandle& EffectContextHandle, const FVector& InKnockBackForce)
{
	if (FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast< FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraGameplayEffectContext->SetKnockBackForce(InKnockBackForce);
	}
}

void UAuraAbilitySystemLibrary::SetIsRadialDamage(FGameplayEffectContextHandle& EffectContextHandle,
	bool bInIsRadialDamage)
{
	if (FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast< FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraGameplayEffectContext->SetIsRadialDamage(bInIsRadialDamage);
	}
}

void UAuraAbilitySystemLibrary::SetRadialDamageInnerRadius(FGameplayEffectContextHandle& EffectContextHandle,
	float InRadialDamageInnerRadius)
{
	if (FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast< FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraGameplayEffectContext->SetRadialDamageInnerRadius(InRadialDamageInnerRadius);
	}
}

void UAuraAbilitySystemLibrary::SetRadialDamageOuterRadius(FGameplayEffectContextHandle& EffectContextHandle,
	float InRadialDamageOuterRadius)
{
	if (FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast< FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraGameplayEffectContext->SetRadialDamageOuterRadius(InRadialDamageOuterRadius);
	}
}

void UAuraAbilitySystemLibrary::SetRadialDamageOrigin(FGameplayEffectContextHandle& EffectContextHandle,
	const FVector& InRadialDamageOrigin)
{
	if (FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast< FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraGameplayEffectContext->SetRadialDamageOrigin(InRadialDamageOrigin);
	}
}

void UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject,
                                                           TArray<AActor*>& OutOverlappedActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);

	TArray<FOverlapResult> OverlapsResults;
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(OverlapsResults, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
		for(FOverlapResult& Overlap: OverlapsResults)
		{
			if (Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Overlap.GetActor()))
			{
				OutOverlappedActors.AddUnique(ICombatInterface::Execute_GetAvatar(Overlap.GetActor()));
			}
		}
	}
}

void UAuraAbilitySystemLibrary::GetClosestTargets(int32 MaxTargets, const TArray<AActor*>& Actors, TArray<AActor*>& OutClosetsTargets, const FVector& Origin)
{
	if (Actors.Num()<=MaxTargets)
	{
		OutClosetsTargets = Actors;
		return;
	}
	TArray<AActor*> ActorsToCheck = Actors;
	int32 NumTargetsFound = 0;
	while(NumTargetsFound < MaxTargets)
	{
		if (ActorsToCheck.Num() == 0) break;
		double ClosestDistance = TNumericLimits<double>::Max();
		AActor* ClosestActor;
		for(AActor* PotentialTarget : ActorsToCheck)
		{
			const double Distance = (PotentialTarget->GetActorLocation() - Origin).Length();
			if (Distance < ClosestDistance)
			{
				ClosestDistance = Distance;
				ClosestActor = PotentialTarget;
			}
		}
		ActorsToCheck.Remove(ClosestActor);
		OutClosetsTargets.AddUnique(ClosestActor);
		++NumTargetsFound;
	}
}

bool UAuraAbilitySystemLibrary::IsNotFriend(AActor* FirstActor, AActor* SecondActor)
{
	const bool bBothArePlayers = FirstActor->ActorHasTag(FName("Player")) && SecondActor->ActorHasTag(FName("Player"));
	const bool bBothAreEnemies = FirstActor->ActorHasTag(FName("Enemy")) && SecondActor->ActorHasTag(FName("Enemy"));
	const bool bFriends = bBothAreEnemies || bBothArePlayers;
	return !bFriends;
}

FGameplayEffectContextHandle UAuraAbilitySystemLibrary::ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams)
{
	const FAuraGameplayTags AuraGameplayTags = FAuraGameplayTags::Get();
	const AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	
	FGameplayEffectContextHandle EffectContextHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(SourceAvatarActor);
	SetDeathImpulse(EffectContextHandle, DamageEffectParams.DeathImpulse);
	SetKnockBackForce(EffectContextHandle, DamageEffectParams.KnockBackForce);
	
	SetIsRadialDamage(EffectContextHandle, DamageEffectParams.bIsRadialDamage);
	SetRadialDamageInnerRadius(EffectContextHandle, DamageEffectParams.RadialDamageInnerRadius);
	SetRadialDamageOuterRadius(EffectContextHandle, DamageEffectParams.RadialDamageOuterRadius);
	SetRadialDamageOrigin(EffectContextHandle, DamageEffectParams.RadialDamageOrigin);

	const FGameplayEffectSpecHandle SpecHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectParams.DamageGameplayEffectClass, DamageEffectParams.AbilityLevel,EffectContextHandle);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageEffectParams.DamageType, DamageEffectParams.BaseDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, AuraGameplayTags.DeBuff_Chance, DamageEffectParams.DeBuffChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, AuraGameplayTags.DeBuff_Damage, DamageEffectParams.DeBuffDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, AuraGameplayTags.DeBuff_Duration, DamageEffectParams.DeBuffDuration);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, AuraGameplayTags.DeBuff_Frequency, DamageEffectParams.DeBuffFrequency);
	
	DamageEffectParams.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
	return EffectContextHandle;
}

void UAuraAbilitySystemLibrary::SetIsRadialDamageEffectParam(FDamageEffectParams& DamageEffectParams, bool bIsRadial, float InnerRadius, float OuterRadius, FVector Origin)
{
	DamageEffectParams.bIsRadialDamage = bIsRadial;
	DamageEffectParams.RadialDamageInnerRadius = InnerRadius;
	DamageEffectParams.RadialDamageOuterRadius = OuterRadius;
	DamageEffectParams.RadialDamageOrigin = Origin;
}

void UAuraAbilitySystemLibrary::SetEffectParamsTargetAsc(FDamageEffectParams& DamageEffectParams, UAbilitySystemComponent* InTargetAsc)
{
	DamageEffectParams.TargetAbilitySystemComponent = InTargetAsc;
}

void UAuraAbilitySystemLibrary::SetKnockBackDirection(FDamageEffectParams& DamageEffectParams, FVector KnockBackDirection, float Magnitude)
{
	KnockBackDirection.Normalize();
	if (Magnitude>0.f)
	{
		DamageEffectParams.KnockBackForce = KnockBackDirection*Magnitude;
	}
	else
	{
		DamageEffectParams.KnockBackForce = KnockBackDirection*DamageEffectParams.KnockBackForceMagnitude;	
	}
}

void UAuraAbilitySystemLibrary::SetDeathImpulseDirection(FDamageEffectParams& DamageEffectParams, FVector DeathImpulseDirection, float Magnitude)
{
	DeathImpulseDirection.Normalize();
	if (Magnitude>0.f)
	{
		DamageEffectParams.DeathImpulse = DeathImpulseDirection*Magnitude;
	}
	else
	{
		DamageEffectParams.DeathImpulse = DeathImpulseDirection*DamageEffectParams.DeathImpulseMagnitude;	
	}
}

TArray<FRotator> UAuraAbilitySystemLibrary::EvenlySpreadRotators(const FVector& Forward, const FVector& Axis, float Spread, int32 NumRotators)
{
	TArray<FRotator> Rotators;
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread/2.f, Axis);
	if (NumRotators>1)
	{
		const float DeltaSpread = Spread / (NumRotators-1);
		for(int32 i=0; i<NumRotators;i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread*i, Axis);
			Rotators.Add(Direction.Rotation());
		}
	}
	else
	{
		Rotators.Add(Forward.Rotation());
	}
	return Rotators;
}

TArray<FVector> UAuraAbilitySystemLibrary::EvenlySpreadVectors(const FVector& Forward, const FVector& Axis, float Spread, int32 NumVectors)
{
	TArray<FVector> Vectors;
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread/2.f, Axis);
	if (NumVectors>1)
	{
		const float DeltaSpread = Spread / (NumVectors-1);
		for(int32 i=0; i<NumVectors;i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread*i, Axis);
			Vectors.Add(Direction);
		}
	}
	else
	{
		Vectors.Add(Forward);
	}
	return Vectors;
}
