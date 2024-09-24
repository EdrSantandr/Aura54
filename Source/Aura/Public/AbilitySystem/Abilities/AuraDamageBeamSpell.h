// EdrSantandr Dev

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "AuraDamageBeamSpell.generated.h"

struct FSortByDistance
{
	explicit FSortByDistance(const FVector& InSourceLocation)
		: SourceLocation(InSourceLocation) {}

	/* The Location to use in our Sort comparison. */
	FVector SourceLocation = FVector::Zero();

	bool operator()(const AActor* A, const AActor* B) const
	{
		float DistanceA = FVector::DistSquared(SourceLocation, A->GetActorLocation());
		float DistanceB = FVector::DistSquared(SourceLocation, B->GetActorLocation());

		return DistanceA < DistanceB;
	}
};

/**
 * 
 */
UCLASS()
class AURA_API UAuraDamageBeamSpell : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void StoreMouseDataInfo(const FHitResult& HitResult);

	UFUNCTION(BlueprintCallable)
	void ElectricStoreMouseDataInfo(AActor* InActor);

	UFUNCTION(BlueprintCallable)
	void TraceFirstTarget(const FVector& BeamTargetLocation);

	UFUNCTION(BlueprintCallable)
	void SentinelTraceFirstTarget(const FVector& InBeamOriginLocation, const FVector& BeamTargetLocation);

	UFUNCTION(BlueprintCallable)
	void SentinelSetHitActor(AActor* InActor);

	UFUNCTION(BlueprintCallable)
	void StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets);

	UFUNCTION(BlueprintImplementableEvent)
	void PrimaryTargetDied(AActor* DeadActor);

	UFUNCTION(BlueprintImplementableEvent)
	void AdditionalTargetDied(AActor* DeadActor);
	
protected:
	UPROPERTY(BlueprintReadWrite, Category="Beam")
	FVector MouseHitLocation;

	UPROPERTY(BlueprintReadWrite, Category="Beam")
	TObjectPtr<AActor> MouseHitActor;
	
	UPROPERTY(EditDefaultsOnly, Category="Beam")
	int32 MaxNumTargets = 5;
};
