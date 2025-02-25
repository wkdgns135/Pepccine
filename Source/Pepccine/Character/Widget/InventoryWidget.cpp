#include "InventoryWidget.h"
#include "Components/UniformGridPanel.h"
#include "InventoryItemWidget.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UInventoryWidget::AddItemToInventory(UTexture2D* ItemImage, const FString& ItemName)
{
	if (!InventoryGrid || !ItemWidgetClass) return;

	UInventoryItemWidget* NewItem = CreateWidget<UInventoryItemWidget>(this, ItemWidgetClass);
	if (NewItem)
	{
		NewItem->SetItem(ItemImage, ItemName);

		UUniformGridSlot* GridSlot = InventoryGrid->AddChildToUniformGrid(NewItem, CurrentRow, CurrentColumn);
        
		if (++CurrentColumn >= 5)
		{
			CurrentColumn = 0;
			++CurrentRow;
		}
	}
}