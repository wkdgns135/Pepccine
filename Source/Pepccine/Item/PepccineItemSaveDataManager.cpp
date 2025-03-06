#include "PepccineItemSaveDataManager.h"

#include "PepccineItemSaveData.h"
#include "Kismet/GameplayStatics.h"

void UPepccineItemSaveDataManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UPepccineItemSaveData* SaveData = Cast<UPepccineItemSaveData>(
		UGameplayStatics::LoadGameFromSlot(TEXT("ItemSaveData"), 0));

	// 세이브 데이터가 없다면 새로 생성
	if (!SaveData)
	{
		SaveData = Cast<UPepccineItemSaveData>(
		UGameplayStatics::CreateSaveGameObject(UPepccineItemSaveData::StaticClass()));
	}
	else
	{
		// 아이템 데이터 초기화
		SaveData->ItemSaveData = FPepccineItemSaveDataStruct();
	}

	UGameplayStatics::SaveGameToSlot(SaveData, TEXT("ItemSaveData"), 0);
}
