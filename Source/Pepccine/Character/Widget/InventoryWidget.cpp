#include "InventoryWidget.h"
#include "InventoryItemWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!InventoryGrid)
	{
		UE_LOG(LogTemp, Error, TEXT("InventoryGrid is nullptr! Make sure it's bound in the widget blueprint."));
	}

	if (!InventoryScrollBox)
	{
		UE_LOG(LogTemp, Error, TEXT("InventoryScrollBox is nullptr! Make sure it's bound in the widget blueprint."));
	}

	if (!ItemWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("ItemWidgetClass is nullptr! Ensure it is assigned in the blueprint or loaded properly."));
	}

	if (!BackgroundImage)
	{
		UE_LOG(LogTemp, Error, TEXT("BackgroundImage is nullptr! Ensure it is assigned in the blueprint or loaded properly."));
	}

	if (!ItemDetailText)
	{
		UE_LOG(LogTemp, Error, TEXT("ItemDetailText is nullptr! Ensure it is assigned in the blueprint."));
	}
	else
	{
		ItemDetailText->SetText(FText::FromString(""));
		ItemDetailText->SetVisibility(ESlateVisibility::Hidden);
	}

	if (!PlayerStatText)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerStatText is nullptr! Ensure it is assigned in the blueprint."));
	}
	else
	{
		PlayerStatText->SetText(FText::FromString(""));
		PlayerStatText->SetVisibility(ESlateVisibility::Hidden);
	}

	CurrentRow = 0;
	CurrentColumn = 0;
	NextItemIndex = 0;

	SetEmptyGrid();
}

void UInventoryWidget::SetEmptyGrid()
{
	for (int32 Row = 0; Row < MaxRows; Row++)
	{
		for (int32 Col = 0; Col < MaxColumns; Col++)
		{
			UInventoryItemWidget* EmptySlot = CreateWidget<UInventoryItemWidget>(this, ItemWidgetClass);
			if (!EmptySlot) continue;
            
			EmptySlot->SetEmpty();

			USizeBox* ItemSizeBox = NewObject<USizeBox>(this);
			if (ItemSizeBox)
			{
				ItemSizeBox->SetWidthOverride(BoxWidthSize);
				ItemSizeBox->SetHeightOverride(BoxHeightSize);
				ItemSizeBox->AddChild(EmptySlot);
			}

			if (UUniformGridSlot* GridSlot = InventoryGrid->AddChildToUniformGrid(ItemSizeBox, Row, Col))
			{
				GridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
				GridSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
			}

			EmptySlot->OnItemHovered.AddDynamic(this, &UInventoryWidget::HandleItemHovered);
		}
	}
}

void UInventoryWidget::AddItemToInventory(UTexture2D* ItemImage, const FString& ItemName, const FString& ItemDetail, const FString& StatDetail)
{
	if (!InventoryGrid || !ItemWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryGrid or ItemWidgetClass is nullptr!"));
		return;
	}

	if (PlayerStatText)
	{
		PlayerStatText->SetText(FText::FromString(*StatDetail));
		PlayerStatText->SetVisibility(ESlateVisibility::Visible);
	}

	UInventoryItemWidget* NewItem = CreateWidget<UInventoryItemWidget>(this, ItemWidgetClass);
	if (!NewItem)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create InventoryItemWidget!"));
		return;
	}

	int32 AssignedIndex = NextItemIndex++;
	ItemDetailsMap.Add(AssignedIndex, ItemDetail);
	
	NewItem->SetItem(ItemImage, ItemName, AssignedIndex);
	NewItem->bIsEmpty = false;

	USizeBox* ItemSizeBox = NewObject<USizeBox>(this);
	if (ItemSizeBox)
	{
		ItemSizeBox->SetWidthOverride(BoxWidthSize);
		ItemSizeBox->SetHeightOverride(BoxHeightSize);
		ItemSizeBox->AddChild(NewItem);
	}

	if (UUniformGridSlot* GridSlot = InventoryGrid->AddChildToUniformGrid(ItemSizeBox, CurrentRow, CurrentColumn))
	{
		GridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
		GridSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
	}

	NewItem->OnItemHovered.AddDynamic(this, &UInventoryWidget::HandleItemHovered);

	CurrentColumn++;
	if (CurrentColumn >= MaxColumns)
	{
		CurrentColumn = 0;
		CurrentRow++;

		if (CurrentRow >= MaxRows) 
		{
			CreateNewGridBlock();
			CurrentRow = 0;
		}
	}
}

void UInventoryWidget::CreateNewGridBlock()
{
	for (int32 Row = 0; Row < MaxRows; Row++)
	{
		for (int32 Col = 0; Col < MaxColumns; Col++)
		{
			UInventoryItemWidget* EmptySlot = CreateWidget<UInventoryItemWidget>(this, ItemWidgetClass);
			if (!EmptySlot) continue;

			EmptySlot->SetEmpty();

			USizeBox* ItemSizeBox = NewObject<USizeBox>(this);
			if (ItemSizeBox)
			{
				ItemSizeBox->SetWidthOverride(BoxWidthSize);
				ItemSizeBox->SetHeightOverride(BoxHeightSize);
				ItemSizeBox->AddChild(EmptySlot);
			}

			if (UUniformGridSlot* GridSlot = InventoryGrid->AddChildToUniformGrid(ItemSizeBox, CurrentRow + Row, Col))
			{
				GridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
				GridSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
			}

			EmptySlot->OnItemHovered.AddDynamic(this, &UInventoryWidget::HandleItemHovered);
		}
	}
}

void UInventoryWidget::HandleItemHovered(int32 ItemIndex, bool bIsHovered)
{
	if (bIsHovered)
	{
		if (FString* FoundDetail = ItemDetailsMap.Find(ItemIndex))
		{
			if (ItemDetailText)
			{
				ItemDetailText->SetText(FText::FromString(*FoundDetail));
				ItemDetailText->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}
	else
	{
		if (ItemDetailText)
		{
			ItemDetailText->SetText(FText::FromString(""));
			ItemDetailText->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}