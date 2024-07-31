// EdrSantandr Dev


#include "AbilitySystem/DeBuff/DeBuffNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"

UDeBuffNiagaraComponent::UDeBuffNiagaraComponent()
{
	bAutoActivate = false;
}

void UDeBuffNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner());

	if (UAbilitySystemComponent* Asc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner()))
	{
		Asc->RegisterGameplayTagEvent(DeBuffTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UDeBuffNiagaraComponent::DeBuffTagChanged);
	}
	else if (CombatInterface)
	{
		CombatInterface->GetOnAscRegisteredDelegate().AddWeakLambda(this,
			[this](UAbilitySystemComponent* InAsc)
			{
				InAsc->RegisterGameplayTagEvent(DeBuffTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UDeBuffNiagaraComponent::DeBuffTagChanged);
			});
	}
	if (CombatInterface)
	{
		CombatInterface->GetOnDeathDelegate().AddDynamic(this, &UDeBuffNiagaraComponent::OnOwnerDeath);
	}
}

void UDeBuffNiagaraComponent::DeBuffTagChanged(const FGameplayTag CallBackTag, int32 NewCount)
{
	const bool bOwnerValid = IsValid(GetOwner());
	const bool bOwnerAlive = GetOwner()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(GetOwner());
	
	if (NewCount>0 && bOwnerValid && bOwnerAlive)
	{
		Activate();
	}
	else
	{
		Deactivate();
	}
}

void UDeBuffNiagaraComponent::OnOwnerDeath(AActor* DeadActor)
{
	Deactivate();
}
