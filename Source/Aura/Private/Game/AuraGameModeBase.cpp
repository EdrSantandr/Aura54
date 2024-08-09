// EdrSantander Dev


#include "Game/AuraGameModeBase.h"

#include "EngineUtils.h"
#include "Game/AuraGameInstance.h"
#include "Game/LoadScreenSaveGame.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/SaveInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

void AAuraGameModeBase::SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex)
{
	//Delete the current if exist
	if (UGameplayStatics::DoesSaveGameExist(LoadSlot->GetLoadSlotName(),SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(LoadSlot->GetLoadSlotName(),SlotIndex);
	}
	//Create a new game save object
	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	if (ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(SaveGameObject))
	{
		LoadScreenSaveGame->PlayerName = LoadSlot->GetPlayerName();
		LoadScreenSaveGame->SaveSlotStatus = Taken;
		LoadScreenSaveGame->MapName = LoadSlot->GetMapName();
		LoadScreenSaveGame->MapAssetName = LoadSlot->MapAssetName;
		LoadScreenSaveGame->PlayerStartTag = LoadSlot->PlayerStartTag;
		UGameplayStatics::SaveGameToSlot(LoadScreenSaveGame, LoadSlot->GetLoadSlotName(), SlotIndex);
	}
}

ULoadScreenSaveGame* AAuraGameModeBase::GetSavedSlotData(const FString& SlotName, int32 SlotIndex) const
{
	USaveGame* SaveGameObject;
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		SaveGameObject = UGameplayStatics::LoadGameFromSlot(SlotName, SlotIndex);
	}
	else
	{
		SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	}
	ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(SaveGameObject);
	return LoadScreenSaveGame;
}

void AAuraGameModeBase::DeleteSlot(const FString& SlotName, int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName,SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(SlotName,SlotIndex);
	}
}

ULoadScreenSaveGame* AAuraGameModeBase::RetrieveInGameSaveData() const
{
	if (const UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance()))
	{
		const FString InGameLoadSlotName = AuraGameInstance->LoadSlotName;
		const int32 InGameLoadSlotIndex = AuraGameInstance->LoadSlotIndex;

		return GetSavedSlotData(InGameLoadSlotName, InGameLoadSlotIndex);
	}
	return nullptr;
}

void AAuraGameModeBase::SaveInGameProgressData(ULoadScreenSaveGame* SaveObject)
{
	if (UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance()))
	{
		const FString InGameLoadSlotName = AuraGameInstance->LoadSlotName;
		const int32 InGameLoadSlotIndex = AuraGameInstance->LoadSlotIndex;
		AuraGameInstance->PlayerStartTag = SaveObject->PlayerStartTag;
		UGameplayStatics::SaveGameToSlot(SaveObject, InGameLoadSlotName, InGameLoadSlotIndex);
	}
}

void AAuraGameModeBase::SaveWorldState(UWorld* World, const FString& DestinationMapAssetName) const
{
	FString WorldName = World->GetMapName();
	WorldName.RemoveFromStart(World->StreamingLevelsPrefix);

	if (UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance()))
	{
		if (ULoadScreenSaveGame* SaveGame = GetSavedSlotData(AuraGameInstance->LoadSlotName, AuraGameInstance->LoadSlotIndex))
		{
			//Check destination string
			if (DestinationMapAssetName != FString(""))
			{
				SaveGame->MapAssetName = DestinationMapAssetName;
				SaveGame->MapName = GetMapNameFromMapAssetName(DestinationMapAssetName);
			}
			// Just the first time
			if (!SaveGame->HasMap(WorldName))
			{
				FSavedMap NewSavedMap;
				NewSavedMap.MapAssetName = WorldName;
				SaveGame->SavedMaps.Add(NewSavedMap);
			}
			FSavedMap SavedMap = SaveGame->GetSavedMapWithMapName(WorldName);
			SavedMap.SavedActors.Empty(); //Clear out, we'll fill it in with "actors"
			// Filling the new data from the map
			for(FActorIterator It(World); It; ++It)
			{
				AActor* Actor = *It;
				if (!IsValid(Actor) || !Actor->Implements<USaveInterface>()) continue;

				FSavedActor SavedActor;
				SavedActor.ActorName = Actor->GetFName();
				SavedActor.Transform = Actor->GetTransform();

				FMemoryWriter MemoryWriter(SavedActor.Bytes);
				FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);
				Archive.ArIsSaveGame = true;
				Actor->Serialize(Archive);
				SavedMap.SavedActors.AddUnique(SavedActor);
			}
			//Replaciong the existing data on the map
			for(FSavedMap& MapToReplace : SaveGame->SavedMaps)
			{
				if (MapToReplace.MapAssetName == WorldName)
				{
					MapToReplace = SavedMap;
				}
			}
			//Finally save the game
			UGameplayStatics::SaveGameToSlot(SaveGame, AuraGameInstance->LoadSlotName, AuraGameInstance->LoadSlotIndex);
		}
	}
}

void AAuraGameModeBase::LoadWorldState(UWorld* World) const
{
	FString WorldName = World->GetMapName();
	WorldName.RemoveFromStart(World->StreamingLevelsPrefix);
	if (UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance()))
	{
		if (UGameplayStatics::DoesSaveGameExist(AuraGameInstance->LoadSlotName, AuraGameInstance->LoadSlotIndex))
		{
			ULoadScreenSaveGame* SaveGame = Cast<ULoadScreenSaveGame>(UGameplayStatics::LoadGameFromSlot(AuraGameInstance->LoadSlotName, AuraGameInstance->LoadSlotIndex));
			if (SaveGame==nullptr)
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to load slot"));
				return;
			}
			for(FActorIterator It(World); It; ++It)
			{
				AActor* Actor = *It;

				if (!Actor->Implements<USaveInterface>()) continue;

				for(FSavedActor SavedActor : SaveGame->GetSavedMapWithMapName(WorldName).SavedActors)
				{
					if (SavedActor.ActorName == Actor->GetName())
					{
						if (ISaveInterface::Execute_ShouldLoadTransform(Actor))
						{
							Actor->SetActorTransform(SavedActor.Transform);
						}
						FMemoryReader MemoryReader(SavedActor.Bytes);

						FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);
						Archive.ArIsSaveGame = true;
						Actor->Serialize(Archive); // converts binery bytes back into variables
						ISaveInterface::Execute_LoadActor(Actor);
					}
				}
			}
		}
	}
}

void AAuraGameModeBase::TravelToMap(UMVVM_LoadSlot* Slot)
{
	const FString SlotName = Slot->GetLoadSlotName();
	const int32 SlotIndex = Slot->SlotIndex;
	
	UGameplayStatics::OpenLevelBySoftObjectPtr(Slot, Maps.FindChecked(Slot->GetMapName()));
}

FString AAuraGameModeBase::GetMapNameFromMapAssetName(const FString& MapAssetName) const
{
	for(auto & Map : Maps)
	{
		if (Map.Value.ToSoftObjectPath().GetAssetName() == MapAssetName)
		{
			return Map.Key;
		}
	}
	return FString();
}

AActor* AAuraGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	if (UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance()))
	{
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Actors);
		if (Actors.Num() > 0)
		{
			AActor* SelectedActor = Actors[0];
			for(AActor* Actor : Actors)
			{
				if (APlayerStart* PlayerStart = Cast<APlayerStart>(Actor))
				{
					if (PlayerStart->PlayerStartTag == AuraGameInstance->PlayerStartTag)
					{
						SelectedActor = PlayerStart;
						break;
					}	
				}
			}
			return SelectedActor;
		}
	}
	return nullptr;
}

void AAuraGameModeBase::PlayerDied(const ACharacter* DeadCharacter) const
{
	ULoadScreenSaveGame* SaveGame = RetrieveInGameSaveData();
	if (!IsValid(SaveGame)) return;

	UGameplayStatics::OpenLevel(DeadCharacter, FName(SaveGame->MapAssetName));
}

void AAuraGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	Maps.Add(DefaultMapName,DefaultMap);
}
