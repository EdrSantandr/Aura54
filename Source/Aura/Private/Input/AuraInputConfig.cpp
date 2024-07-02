// EdrSantander Dev


#include "Input/AuraInputConfig.h"

const UInputAction* UAuraInputConfig::FindAbilityInputActionByTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for(const FAuraInputAction Action : AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.MatchesTagExact(InputTag))
		{
			return Action.InputAction;
		}
	}
	if(bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("can't find ablity input action for input tag [%s] on inputconfig [%s]"), *InputTag.ToString(), *GetNameSafe(this));
	}
	return nullptr;
}
