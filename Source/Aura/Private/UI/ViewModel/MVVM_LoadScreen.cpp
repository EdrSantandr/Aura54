// EdrSantandr Dev


#include "UI/ViewModel/MVVM_LoadScreen.h"

#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadScreen::InitializeLoadSlots()
{
	LoadSlot_0 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_0->SetLoadSlotName(FString("LoadSlot_0"));
	LoadSlot_0->SlotIndex = 0;
	LoadSlots.Add(0, LoadSlot_0);
	LoadSlot_1 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_1->SetLoadSlotName(FString("LoadSlot_1"));
	LoadSlot_1->SlotIndex = 1;
	LoadSlots.Add(1, LoadSlot_1);
	LoadSlot_2 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_2->SetLoadSlotName(FString("LoadSlot_2"));
	LoadSlot_2->SlotIndex = 2;
	LoadSlots.Add(2, LoadSlot_2);

	SetNumLoadSlots(LoadSlots.Num());
}

UMVVM_LoadSlot* UMVVM_LoadScreen::GetLoadSlotViewModelByIndex(int32 Index) const
{
	return LoadSlots.FindChecked(Index);
}

void UMVVM_LoadScreen::NewSlotButtonPressed(int32 Slot, const FString EnteredName)
{
	if (AAuraGameModeBase* AuraGameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		LoadSlots[Slot]->SetMapName(AuraGameModeBase->DefaultMapName);
		LoadSlots[Slot]->SetPlayerName(EnteredName);
		LoadSlots[Slot]->SlotStatus = Taken;
		AuraGameModeBase->SaveSlotData(LoadSlots[Slot], Slot);
		LoadSlots[Slot]->InitializeSlot();
	}
}

void UMVVM_LoadScreen::NewGameButtonPressed(int32 Slot)
{
	LoadSlots[Slot]->SetWidgetSwitcherIndexDelegate.Broadcast(1);
}

void UMVVM_LoadScreen::SelectSlotButtonPressed(int32 Slot)
{
	SlotSelectedDelegate.Broadcast();
	for (const TTuple<int32, UMVVM_LoadSlot*> LoadSlot : LoadSlots)
	{
		if (LoadSlot.Key == Slot)
		{
			LoadSlot.Value->EnableSelectSlotButtonDelegate.Broadcast(false);	
		}
		else
		{
			LoadSlot.Value->EnableSelectSlotButtonDelegate.Broadcast(true);
		}
	}
	SelectedSlot = LoadSlots[Slot];
}

void UMVVM_LoadScreen::DeleteButtonPressed()
{
	if(IsValid(SelectedSlot))
	{
		AAuraGameModeBase::DeleteSlot(SelectedSlot->GetLoadSlotName(), SelectedSlot->SlotIndex);
		SelectedSlot->SlotStatus = Vacant;
		SelectedSlot->InitializeSlot();
		SelectedSlot->EnableSelectSlotButtonDelegate.Broadcast(true);
	}
}

void UMVVM_LoadScreen::PlayButtonPressed()
{
	if (AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		if(IsValid(SelectedSlot))
		{
			AuraGameMode->TravelToMap(SelectedSlot);	
		}
	} 
}

void UMVVM_LoadScreen::LoadData()
{
	if (AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		for (const TTuple<int32, UMVVM_LoadSlot*> LoadSlot : LoadSlots)
		{
			if ( const ULoadScreenSaveGame* SaveObject = AuraGameMode->GetSavedSlotData(LoadSlot.Value->GetLoadSlotName(), LoadSlot.Key))
			{
				const FString PlayerName = SaveObject->PlayerName;
				const TEnumAsByte<ESaveSlotStatus> SaveSlotStatus = SaveObject->SaveSlotStatus;
			
				LoadSlot.Value->SlotStatus = SaveSlotStatus;
				LoadSlot.Value->SetPlayerName(PlayerName);
				LoadSlot.Value->SetMapName(SaveObject->MapName);
				LoadSlot.Value->InitializeSlot();	
			}
		}
	} 
}

void UMVVM_LoadScreen::SetNumLoadSlots(int32 InNumLoadSlots)
{
	UE_MVVM_SET_PROPERTY_VALUE(NumLoadSlots, InNumLoadSlots);
}
