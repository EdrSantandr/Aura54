// EdrSantandr Dev


#include "AbilitySystem/Data/MessageInfo.h"

#include "Aura/AuraLogChannels.h"

FMessageInfo UPlayerMessageInfo::FindMessagePlayerInfoByTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	for(const FMessageInfo& Info : MessagePlayerInformation)
	{
		if (Info.MessageTag.MatchesTagExact(AttributeTag))
		{
			return Info;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogAura, Error, TEXT("Cant find info for attribute tag [%s] on AttributeInfo [%s]"), *AttributeTag.ToString(),*GetNameSafe(this));
	}
	return FMessageInfo();
}
