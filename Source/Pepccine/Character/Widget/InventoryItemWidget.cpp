#include "InventoryItemWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

#include "Styling/SlateBrush.h"
#include "Styling/SlateTypes.h"

void UInventoryItemWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ItemButton)
	{
		ItemButton->OnHovered.AddDynamic(this, &UInventoryItemWidget::OnHovered);
		ItemButton->OnUnhovered.AddDynamic(this, &UInventoryItemWidget::OnUnhovered);
	}

	if (bIsEmpty)
	{
		SetEmpty();
	}
}

void UInventoryItemWidget::SetEmpty()
{
	if (ItemImageWidget)
	{
		ItemImageWidget->SetBrushFromTexture(nullptr);
		ItemImageWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	
	if (TooltipInfo)
	{
		TooltipInfo->SetText(FText::FromString(""));
		TooltipInfo->SetVisibility(ESlateVisibility::Hidden);
	}

	bIsEmpty = true;
}

void UInventoryItemWidget::SetItem(UTexture2D* ItemImage, const FString& ItemName)
{
	if (TooltipInfo)
	{
		TooltipInfo->SetText(FText::FromString(ItemName));
		TooltipInfo->SetVisibility(ESlateVisibility::Hidden);
	}
	
	if (ItemImageWidget && ItemImage)
	{
		//ItemImageWidget->SetToolTipText(FText::FromString(ItemName));
		ItemImageWidget->SetBrushFromTexture(ItemImage);
		ItemImageWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void UInventoryItemWidget::OnHovered()
{
	if (TooltipInfo)
	{
		UE_LOG(LogTemp, Display, TEXT("OnHovered"));
		TooltipInfo->SetVisibility(ESlateVisibility::Visible);
	}
}

void UInventoryItemWidget::OnUnhovered()
{
	if (TooltipInfo)
	{
		UE_LOG(LogTemp, Display, TEXT("OnUnhovered"));
		TooltipInfo->SetVisibility(ESlateVisibility::Hidden);
	}
}