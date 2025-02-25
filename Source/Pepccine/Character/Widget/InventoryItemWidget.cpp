#include "InventoryItemWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

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
	if (TooltipInfo)
	{
		TooltipInfo->SetText(FText::FromString(ItemName));
		TooltipInfo->SetVisibility(ESlateVisibility::Hidden);
	}

	if (ItemImageWidget && ItemImage)
	{
		ItemImageWidget->SetBrushFromTexture(ItemImage);
		ItemImageWidget->SetVisibility(ESlateVisibility::Visible);

		ItemImageWidget->SetColorAndOpacity(FLinearColor(1, 0, 0, 1));
		UE_LOG(LogTemp, Log, TEXT("Item Image Set Successfully: %s"), *ItemName);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemImageWidget or ItemImage is nullptr!"));
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