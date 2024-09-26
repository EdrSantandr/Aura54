// EdrSantander Dev

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AuraGameModeBase.generated.h"

class UPlayerMessageInfo;
class ULootTiersInfo;
class ULoadScreenSaveGame;
class USaveGame;
class UMVVM_LoadSlot;
class UAbilityInfo;
class UCharacterClassInfo;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnGateDestroyedSignature, int32 /*CurrentGates*/, int32 /*TotalGates*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnEnemyKilledSignature, int32 /*EnemiesKilled*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerLivesSignature, int32 /*PlayerLives*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAbilityPlayerMessageSignature, const FString& /*StringMessage*/);

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

	UPROPERTY(EditDefaultsOnly, Category="PlayerMessageInfo")
	TObjectPtr<UPlayerMessageInfo> PlayerMessageInfo;

	FOnGateDestroyedSignature OnGateDestroyedDelegate;

	FOnEnemyKilledSignature OnEnemyKilledDelegate;

	FOnPlayerLivesSignature OnPlayerLivesDelegate;

	FOnAbilityPlayerMessageSignature OnAbilityPlayerMessageSignature;
	
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
	void ReduceLivePoints(const int32 InMinusPoints);

	UFUNCTION()
	void IncrementLivePoints(const int32 InPlusPoints);
	
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
	int32 NumberOfLives = 5;

	UPROPERTY(BlueprintReadOnly, Category="LiriaModifiers")
	int32 NumberOfGates = 0;

	UPROPERTY(EditDefaultsOnly, Category="LiriaModifiers")
	FName MainMenuMapName = FName("MainMenu_Liria");

	FString GetMapNameFromMapAssetName(const FString& MapAssetName) const;
	
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	void PlayerDied(const ACharacter* DeadCharacter) const;

	void LiriaPlayerDied(ACharacter* DeadCharacter);

	void RemoveGate(AActor* DeathGate);

	void EnemyKilled();

	UFUNCTION()
	void MainMenuScreen(ACharacter* DeadCharacter);

	UFUNCTION(BlueprintImplementableEvent)
	void ShowMessageYouLose(int32 InEnemiesKilled);

	UFUNCTION(BlueprintImplementableEvent)
	void ShowMessageYouWin(int32 InEnemiesKilled);

	UPROPERTY(EditDefaultsOnly, Category="LiriaModifiers")
	float LoseWaitTime = 5.f;

	UPROPERTY(EditDefaultsOnly, Category="LiriaModifiers")
	float WinWaitTime = 10.f;

protected:
	virtual void BeginPlay() override;

private:

	void ImproveGates(float GatesNewLevel);
	
	void SearchGatesInMap();

	UFUNCTION()
	void FinalEnemiesInteraction(const bool WinOrLoseGame) const;

	UPROPERTY()
	TArray<AActor*> Gates;

	UPROPERTY()
	int32 TotalGates = 0;

	UPROPERTY()
	int32 LiveGates = 0;

	UPROPERTY()
	int32 EnemiesKilled = 0;

	UPROPERTY()
	int32 GatesLevel = 1;

	FTimerHandle PlayerDeadTimerHandle;
	
	FTimerDelegate PlayerDeadTimerDelegate;
	
};
