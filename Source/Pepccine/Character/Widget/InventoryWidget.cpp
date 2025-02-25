#include "InventoryWidget.h"
#include "InventoryItemWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/Image.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!InventoryGrid)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ InventoryGrid is nullptr! Make sure it's bound in the widget blueprint."));
	}

	if (!ItemWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ ItemWidgetClass is nullptr! Ensure it is assigned in the blueprint or loaded properly."));
	}

	if (!BackgroundImage)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ BackgroundImage is nullptr! Ensure it is assigned in the blueprint or loaded properly."));
	}

	SetEmptySpace();
}

void UInventoryWidget::SetEmptySpace()
{
	for (int32 row = 0; row < MaxRows; row++)
	{
		for (int32 col = 0; col < MaxColumns; col++)
		{
			UInventoryItemWidget* EmptySlot = CreateWidget<UInventoryItemWidget>(this, ItemWidgetClass);
			EmptySlot->SetEmpty();
			//InventoryGrid->AddChildToUniformGrid(EmptySlot, row, col);
			if (UUniformGridSlot* GridSlot = InventoryGrid->AddChildToUniformGrid(EmptySlot, row, col))
			{
				GridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
				GridSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
			}
			GridSlots.Add(EmptySlot);
		}
	}
}

void UInventoryWidget::AddItemToInventory(UTexture2D* ItemImage, const FString& ItemName)
{
	if (!InventoryGrid || !ItemWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("❌ InventoryGrid or ItemWidgetClass is nullptr!"));
		return;
	}

	for (int32 i = 0; i < GridSlots.Num(); i++)
	{
		if (GridSlots[i]->bIsEmpty)
		{
			GridSlots[i]->SetItem(ItemImage, ItemName);
			GridSlots[i]->bIsEmpty = false;
			return;
		}
	}
}