// EdrSantander Dev

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "AuraCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UNiagaraComponent;
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
	virtual void Die(const FVector& DeathImpulse) override;

	UPROPERTY(EditDefaultsOnly)
	float DeathTime = 5.f;

	UPROPERTY(EditDefaultsOnly, Category="CameraMovementClamp")
	float MaxLocationTop = 3231.f;

	UPROPERTY(EditDefaultsOnly, Category="CameraMovementClamp")
	float MaxLocationBottom = -1448.f;

	UPROPERTY(EditDefaultsOnly, Category="CameraMovementClamp")
	float MaxLocationRight = 3601.f;

	UPROPERTY(EditDefaultsOnly, Category="CameraMovementClamp")
	float MaxLocationLeft = -388.f;

	FTimerHandle DeathTimerHandle;

	/** Player Interface*/
	virtual void AddToExp_Implementation(int32 InExp) override;
	virtual void LevelUp_Implementation() override;
	virtual int32 GetExp_Implementation() const override;
	virtual int32 FindLevelForExp_Implementation(int32 InExp) const override;
	virtual int32 GetAttributePointsReward_Implementation(int32 Level) const override;
	virtual int32 GetSpellPointsReward_Implementation(int32 Level) const override;
	virtual void AddToPlayerLevel_Implementation(int32 InPlayerLevel) override;
	virtual void AddToSpellPoints_Implementation(int32 InSpellPoints) override;
	virtual void AddToAttributePoints_Implementation(int32 InAttributePoints) override;
	virtual int32 GetAttributePoints_Implementation() const override;
	virtual int32 GetSpellPoints_Implementation() const override;
	virtual void ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial) override;
	virtual void HideMagicCircle_Implementation() override;
	virtual void SaveProgress_Implementation(const FName& CheckPointTag) override;
	virtual void MoveCamera_Implementation(const bool bMoveLeft, const bool bMoveRight, const bool bMoveBottom, const bool bMoveTop, const float CameraSpeed) override;
	virtual void FocusCharacterCamera_Implementation() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;

	virtual void OnRep_Stunned() override;
	virtual void OnRep_Burned() override;
	void LoadProgress();
	
private:
	virtual void InitAbilityActorInfo() override;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastLevelUpParticles() const;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> TopDownCameraComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	static void CheckCameraLimits(float const LowerLimit, float const UpperLimit, float const CameraComponentLocation, double& CameraMovement);
};
