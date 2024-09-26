// EdrSantandr Dev

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "MessageInfo.generated.h"

USTRUCT(BlueprintType)
struct FMessageInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText MessageContent = FText();
};
UCLASS()
class AURA_API UPlayerMessageInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	FMessageInfo FindMessagePlayerInfoByTag(const FGameplayTag& AttributeTag, bool bLogNotFound = false) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FMessageInfo> MessagePlayerInformation;
};
