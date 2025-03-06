#include "PepccineActiveItemManager.h"

#include "PepccineActiveItemData.h"
#include "Item/Manager/PepccineItemManagerComponent.h"

void UPepccineActiveItemManager::PickUpItem(const UPepccineActiveItemData* InActiveItemData)
{
	if (UPepccineActiveItemData* NewActiveItemData = DuplicateObject<UPepccineActiveItemData>(InActiveItemData, this))
	{
		ActiveItemData = NewActiveItemData;

		// 획득시 바로 재사용 대기시간 적용
		ActiveItemRemainingCooldown = ActiveItemData->GetCooldown();
		UE_LOG(LogTemp, Warning, TEXT("액티브 아이템 재사용 대기시간 : %.2f"), ActiveItemRemainingCooldown);
		bIsActiveItemCooldown = true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("액티브 아이템 데이터가 업습니다."));
	}
}

void UPepccineActiveItemManager::UseActiveItem()
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

	// 포션(버프)
	if (const UPepccinePotionItemData* PotionItemData = Cast<UPepccinePotionItemData>(ActiveItemData))
	{
		ActiveItemRemainingCooldown = ActiveItemData->GetCooldown();
		
		// 버프 적용 중일 경우
		if (bIsAppliedBuff)
		{
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
			UE_LOG(LogTemp, Warning, TEXT("버프 갱신!"));
		}
		// 버프 적용 중이 아닐 경우
		else
		{
			ActivatePotionItem(PotionItemData);
		}
		
		bIsActiveItemCooldown = true;
		TimerDelegate.BindUFunction(this, FName("DeactivatePotionItem"), PotionItemData);
		// 지속시간 이후 버프 해제
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, PotionItemData->GetDuration(), false);
	}
}

void UPepccineActiveItemManager::ActivatePotionItem(const UPepccinePotionItemData* PotionItemData)
{
	bIsAppliedBuff = true;
	ItemManager->IncreaseStatsOperations(PotionItemData->GetWeaponStatModifiers());
	ItemManager->IncreaseStatsOperations(PotionItemData->GetCharacterStatModifiers());
	UE_LOG(LogTemp, Warning, TEXT("버프 적용!"));
}

void UPepccineActiveItemManager::DeactivatePotionItem(const UPepccinePotionItemData* PotionItemData)
{
	bIsAppliedBuff = false;
	ItemManager->DecreaseStatsOperations(PotionItemData->GetWeaponStatModifiers());
	ItemManager->DecreaseStatsOperations(PotionItemData->GetCharacterStatModifiers());
	UE_LOG(LogTemp, Warning, TEXT("버프 해제!"));
}
