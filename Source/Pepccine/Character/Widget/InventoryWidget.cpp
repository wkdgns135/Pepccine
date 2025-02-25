#include "InventoryWidget.h"
#include "Components/UniformGridPanel.h"
#include "InventoryItemWidget.h"
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
		UE_LOG(LogTemp, Warning, TEXT("❌ BackgroundImage is nullptr! Check if it's assigned in the widget blueprint."));
	}
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

		UE_LOG(LogTemp, Warning, TEXT("Inventory Widget: [%d][%d]"), CurrentRow, CurrentColumn);
	}
}