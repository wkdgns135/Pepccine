#include "PepccineActiveItemManager.h"

#include "PepccineActiveItemData.h"
#include "Item/PepccineItemManagerComponent.h"

void UPepccineActiveItemManager::PickUpItem(const UPepccineActiveItemData* InActiveItemData)
{
	if (UPepccineActiveItemData* NewActiveItemData = DuplicateObject<UPepccineActiveItemData>(InActiveItemData, this))
	{
		ActiveItemData = NewActiveItemData;

		// 획득시 바로 재사용 대기시간 적용
		ActiveItemRemainingCooldown = ActiveItemData->GetCooldown();
		bIsActiveItemCooldown = true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("액티브 아이템 데이터가 업습니다."));
	}
}

void UPepccineActiveItemManager::UseActiveItem(UPepccineItemManagerComponent* ItemManager)
{
	// 액티브 아이템이 없을 경우
	if (!ActiveItemData)
	{
		UE_LOG(LogTemp, Warning, TEXT("액티브 아이템이 없습니다."));
		return;
	}
	
	// 현재 액티브 아이템 재사용 대기 중일 경우
	if (bIsActiveItemCooldown)
	{
		UE_LOG(LogTemp, Warning, TEXT("액티브 아이템 재사용 대기 중"));
		return;
	}

	// 이미 해당 버프를 받고 있다면 시간만 갱신
	if (GetAppliedPotionItemDataById(ActiveItemData->GetItemId()))
	{
		ActiveItemRemainingCooldown = ActiveItemData->GetCooldown();
		bIsActiveItemCooldown = true;
		return;
	}

	// 포션(버프)
	if (const UPepccinePotionItemData* PotionItemData = Cast<UPepccinePotionItemData>(ActiveItemData))
	{
		ActivatePotionItem(PotionItemData, ItemManager);

		// 지속시간 이후 버프 해제
		FTimerHandle TimerHandle;
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUFunction(this, FName("DeactivatePotionItem"), PotionItemData, ItemManager);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, PotionItemData->GetDuration(), false);
	}
}

void UPepccineActiveItemManager::ActivatePotionItem(const UPepccinePotionItemData* PotionItemData, UPepccineItemManagerComponent* ItemManager)
{
	ItemManager->IncreaseStatsOperations(PotionItemData->GetWeaponStatModifiers());
	ItemManager->IncreaseStatsOperations(PotionItemData->GetCharacterStatModifiers());
}

void UPepccineActiveItemManager::DeactivatePotionItem(const UPepccinePotionItemData* PotionItemData, UPepccineItemManagerComponent* ItemManager)
{
	ItemManager->DecreaseStatsOperations(PotionItemData->GetWeaponStatModifiers());
	ItemManager->DecreaseStatsOperations(PotionItemData->GetCharacterStatModifiers());
}

UPepccinePotionItemData* UPepccineActiveItemManager::GetAppliedPotionItemDataById(const int32 Id) const
{
	for (UPepccinePotionItemData* PotionItemData : AppliedBuffPotionItemDatas)
	{
		if (PotionItemData->GetItemId() == Id)
		{
			return PotionItemData;
		}
	}

	return nullptr;
}
