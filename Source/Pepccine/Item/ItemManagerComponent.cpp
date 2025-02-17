#include "ItemManagerComponent.h"
#include "ItemDataRow.h"

UItemManagerComponent::UItemManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UItemManagerComponent::BeginPlay()
{
	Super::BeginPlay();	

	if (ItemStatDataTable)
	{
		// 모든 행 가져오기
		TArray<FItemDataRow*> AllRows;
		static const FString ContextString(TEXT("ItemStatContext"));
		ItemStatDataTable->GetAllRows(ContextString, AllRows);

		for(const FItemDataRow* Row : AllRows)
		{
			// 행이 유효한지 확인
			if (Row)
			{
				UE_LOG(LogTemp, Warning, TEXT("AttackMultiplier = %.1f / FireRateMultiplier = %.1f"), Row->AttackMultiplier, Row->FireRateMultiplier);
			}
		}
	}
}

void UItemManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

