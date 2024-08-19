// EdrSantander Dev

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AuraGameModeBase.generated.h"

class ULootTiersInfo;
class ULoadScreenSaveGame;
class USaveGame;
class UMVVM_LoadSlot;
class UAbilityInfo;
class UCharacterClassInfo;

/**
 * 
 */
UCLASS()
class AURA_API AAuraGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="CharacterClassDefaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

	UPROPERTY(EditDefaultsOnly, Category="AbilityInfo")
	TObjectPtr<UAbilityInfo> AbilityInfo;

	UPROPERTY(EditDefaultsOnly, Category="LootTiersInfo")
	TObjectPtr<ULootTiersInfo> LootTiersInfo;

	UFUNCTION()
	void SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex);

	ULoadScreenSaveGame* GetSavedSlotData(const FString& SlotName, int32 SlotIndex) const;
	static void DeleteSlot(const FString& SlotName, int32 SlotIndex);
	ULoadScreenSaveGame* RetrieveInGameSaveData() const;
	void SaveInGameProgressData(ULoadScreenSaveGame* SaveObject);

	void SaveWorldState(UWorld* World, const FString& DestinationMapAssetName = FString("")) const;
	void LoadWorldState(UWorld* World) const;

	void TravelToMap(UMVVM_LoadSlot* Slot);

	UFUNCTION()
	int32 GetNumberOfLives() const { return NumberOfLives; }

	UFUNCTION()
	void ReduceLivePoints(const int32 InMinusPoints) { NumberOfLives -=InMinusPoints; }

	UFUNCTION()
	void IncrementLivePoints(const int32 InPlusPoints) { NumberOfLives +=InPlusPoints; }
	
	UPROPERTY(EditDefaultsOnly, Category="SaveInfo")
	TSubclassOf<USaveGame> LoadScreenSaveGameClass;

	UPROPERTY(EditDefaultsOnly, Category="SaveInfo")
	FString DefaultMapName;

	UPROPERTY(EditDefaultsOnly, Category="SaveInfo")
	TSoftObjectPtr<UWorld> DefaultMap;

	UPROPERTY(EditDefaultsOnly, Category="SaveInfo")
	FName DefaultPlayerStartTag;
	
	UPROPERTY(EditDefaultsOnly, Category="SaveInfo")
	TMap<FString, TSoftObjectPtr<UWorld>> Maps;

	UPROPERTY(EditDefaultsOnly, Category="LiriaModifiers")
	int32 NumberOfLives = 20;

	UPROPERTY(EditDefaultsOnly, Category="LiriaModifiers")
	FVector OriginPoint = FVector(0.f,0.f, 0.f);

	UPROPERTY(EditDefaultsOnly, Category="LiriaModifiers")
	FVector FinalPoint = FVector(1500.f,1500.f, 0.f);

	UPROPERTY(EditDefaultsOnly, Category="LiriaModifiers")
	float YPathDimension = 1000.f;

	FString GetMapNameFromMapAssetName(const FString& MapAssetName) const;
	
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	void PlayerDied(const ACharacter* DeadCharacter) const;

	UPROPERTY()
	TArray<AActor*> BestPath;
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void CreatePaths();
};
