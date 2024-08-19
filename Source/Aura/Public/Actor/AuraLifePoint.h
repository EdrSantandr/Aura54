// EdrSantandr Dev

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraLifePoint.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class AURA_API AAuraLifePoint : public AActor
{
	GENERATED_BODY()
	
public:
	AAuraLifePoint();

	UFUNCTION(BlueprintCallable, Category="LifePoint")
	void DestroyLifePoint();

	virtual void Destroyed() override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="LifePoint")
	TObjectPtr<UStaticMeshComponent> LifePointMesh;
	
	UPROPERTY(EditAnywhere, Category="LifePoint")
	TObjectPtr<UNiagaraSystem> DestroyEffect;

	UPROPERTY(EditDefaultsOnly, Category="LifePoint")
	float TimeBeforeDestroy = 2.f;

	UPROPERTY(EditDefaultsOnly, Category="LifePoint")
	float MinScale = 0.90f;

	UPROPERTY(EditDefaultsOnly, Category="LifePoint")
	float MaxScale = 1.1f;

	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;

	UPROPERTY()
	UNiagaraComponent* NiagaraSpawned;

	UFUNCTION()
	void LifePoint_Elapsed();
};
