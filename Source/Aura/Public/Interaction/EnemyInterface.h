// EdrSantander Dev

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnemyInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IEnemyInterface
{
	GENERATED_BODY()

public:
	virtual void HighlightActor() = 0; //=0 makes a pure virtual function
	virtual void UnHighlightActor() = 0;
	
};
