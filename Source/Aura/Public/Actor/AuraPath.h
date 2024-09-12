// EdrSantandr Dev

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/PathInterface.h"
#include "AuraPath.generated.h"

class USphereComponent;
class UBillboardComponent;

UCLASS()
class AURA_API AAuraPath : public AActor, public IPathInterface
{
	GENERATED_BODY()
	
public:	
	AAuraPath();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category="Path")
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(EditDefaultsOnly, Category="Path")
	TObjectPtr<UBillboardComponent> BillboardComponent;
};
