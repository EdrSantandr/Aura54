// EdrSantander Dev

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * Aura gameplayTags - Singleton
 */
struct FAuraGameplayTags
{

public:
	static const FAuraGameplayTags Get() { return GameplayTags;}
	static void InitializeNativeGameplayTags();

	FGameplayTag Attributes_Secondary_Armor;

protected:

private:
	static FAuraGameplayTags GameplayTags;
};
