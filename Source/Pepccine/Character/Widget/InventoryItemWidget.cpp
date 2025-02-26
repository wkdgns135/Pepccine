#include "InventoryItemWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UInventoryItemWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ItemButton)
	{
		ItemButton->OnHovered.AddDynamic(this, &UInventoryItemWidget::OnHovered);
		ItemButton->OnUnhovered.AddDynamic(this, &UInventoryItemWidget::OnUnhovered);
	}
}

void UInventoryItemWidget::SetItem(UTexture2D* ItemImage, const FString& ItemName)
{
	if (ItemImageWidget)
	{
		ItemImageWidget->SetBrushFromTexture(ItemImage);
	}
    
	if (TooltipInfo)
	{
		TooltipInfo->SetText(FText::FromString(ItemName));
		TooltipInfo->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UInventoryItemWidget::OnHovered()
{
	if (TooltipInfo)
	{
		TooltipInfo->SetVisibility(ESlateVisibility::Visible);
	}
}

void UInventoryItemWidget::OnUnhovered()
{
	if (TooltipInfo)
	{
		TooltipInfo->SetVisibility(ESlateVisibility::Hidden);
	}
}