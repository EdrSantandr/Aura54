// EdrSantandr Dev


#include "AbilitySystem/Data/LevelUpInfo.h"

int32 ULevelUpInfo::FindLevelForExp(int32 Exp) const
{
	int32 Level = 1;
	bool bSearching = true;
	while (bSearching)
	{
		// Levelupinformation [1] = Level 1 information
		// Levelupinformation [2] = Level 1 information
		if (LevelUpInformation.Num() -1 <= Level) return Level;

		if(Exp >= LevelUpInformation[Level].LevelUpRequirement)
		{
			++Level;
		}
		else
		{
			bSearching=false;	
		}
	}
	return Level;
}
