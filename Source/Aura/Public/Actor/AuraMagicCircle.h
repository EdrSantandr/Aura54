// EdrSantandr Dev

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraMagicCircle.generated.h"

UCLASS()
class AURA_API AAuraMagicCircle : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraMagicCircle();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="MagicCircleEffect")
	TObjectPtr<UDecalComponent> MagicCircleDecal;

protected:
	virtual void BeginPlay() override;
};
