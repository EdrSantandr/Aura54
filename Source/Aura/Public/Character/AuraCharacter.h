// EdrSantander Dev

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "AuraCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()

public:
	AAuraCharacter();
	
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	/** Combat Interface*/
	virtual int32 GetPlayerLevel_Implementation() override;

	/** Player Interface*/
	virtual void AddToExp_Implementation(int32 InExp) override;
	virtual void LevelUp_Implementation() override;

private:
	virtual void InitAbilityActorInfo() override;
};
