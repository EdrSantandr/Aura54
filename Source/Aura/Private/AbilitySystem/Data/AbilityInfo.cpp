// EdrSantandr Dev


#include "AbilitySystem/Data/AbilityInfo.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Aura/AuraLogChannels.h"

FAuraAbilityInfo UAbilityInfo::FindAbilityInfoForTag(const FGameplayTag& AbilityTag, bool bLogNotFound) const
{
	for(const FAuraAbilityInfo& Info : AbilityInformation)
	{
		if (Info.AbilityTag == AbilityTag)
		{
			return Info;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogAura, Error, TEXT("Can't find info for ability tag [%s] on Ability info [%s]"),*AbilityTag.ToString(),*GetNameSafe(this));
	}
	
	return FAuraAbilityInfo();
}
