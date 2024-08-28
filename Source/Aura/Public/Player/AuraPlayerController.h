// EdrSantander Dev

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "Input/AuraInputComponent.h"
#include "AuraPlayerController.generated.h"


class IHighlightInterface;
class AAuraMagicCircle;
class UNiagaraSystem;
class UDamageTextWidgetComponent;
class USplineComponent;
class UAuraInputConfig;
struct FInputActionValue;
class UInputMappingContext;
class UInputAction;
class UAuraAbilitySystemComponent;

enum class ETargetingStatus : uint8
{
	TargetingEnemy,
	TargetingNonEnemy,
	NotTargeting
};

/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAuraPlayerController();
	
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit);

	UFUNCTION(BlueprintCallable)
	void ShowMagicCircle(UMaterialInterface* DecalMaterial = nullptr);

	UFUNCTION(BlueprintCallable)
	void HideMagicCircle();

	UFUNCTION()
	void BindConfirmAndCancel();

	UFUNCTION()
	void ChangeToConfirmInputMapping() const;

	UFUNCTION()
	void ChangeToLiriaInputMapping() const;

	UFUNCTION()
	AActor* GetHighLightedActor() { return ThisActor; }
	
	UFUNCTION(BlueprintImplementableEvent, Category="MouseCursor")
	void ChangeCursorByActor(AActor* InActorHighlighted);
	
	UFUNCTION(BlueprintImplementableEvent, Category="MouseCursor")
	void ChangeCursorByCasting() const;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="MouseCursor")
	void SetCursorDefault() const;
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> ConfirmContext;
	
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> ShiftAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> F1Action;

	UPROPERTY(EditAnywhere, Category="CameraLimitsPercentage")
	float XCameraLimitPercentage = 0.1f;

	UPROPERTY(EditAnywhere, Category="CameraLimitsPercentage")
	float YCameraLimitPercentage = 0.12f;

	UPROPERTY(EditAnywhere, Category="CameraLimitsPercentage")
	float CameraSpeed = 30.f;
	
	void Move(const FInputActionValue& InputActionValue);

	void ShiftPressed() { bShiftKeyDown = true;}
	void ShiftReleased() { bShiftKeyDown = false;}
	bool bShiftKeyDown = false;

	void F1Pressed() { bF1KeyDown = true;}
	void F1Released();
	bool bF1KeyDown = false;
	
	void CursorTrace();
	void CursorCameraMovement();
	bool CheckCameraLimit(float InMouseCoordinate, float LowerLimit, float UpperLimit);
	void CheckLimitsCoordinate(bool& InLowerLimit, bool& InUpperLimit, float MouseCoordinate, float Percentage, int32 ViewPort);

	UPROPERTY()
	TObjectPtr<AActor> LastActor;

	UPROPERTY()
	TObjectPtr<AActor> ThisActor;
	
	FHitResult CursorHit;

	void HighLightActor(AActor* InActor);
	void UnHighLightActor(AActor* InActor);

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UAuraInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	UAuraAbilitySystemComponent* GetASC();

	/* Camera*/
	bool bLowerXLimit = false;
	bool bUpperXLimit = false;
	bool bLowerYLimit = false;
	bool bUpperYLimit = false;
	
	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	float ShortPressThreshold = 0.5f;
	bool bAutoRunning = false;
	ETargetingStatus TargetingStatus = ETargetingStatus::NotTargeting;

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraSystem>ClickNiagaraSystem;

	void AutoRun();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextWidgetComponent> DamageTextComponentClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AAuraMagicCircle> MagicCircleClass;

	UPROPERTY()
	TObjectPtr<AAuraMagicCircle> MagicCircle = nullptr;

	void UpdateMagicCircleLocation();
};
