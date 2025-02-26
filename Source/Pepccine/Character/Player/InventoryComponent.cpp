#include "InventoryComponent.h"
#include "Character/Widget/InventoryWidget.h"
#include "Blueprint/UserWidget.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	if (InventoryWidgetClass)
	{
		InventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(), InventoryWidgetClass);
		if (InventoryWidget)
		{
			InventoryWidget->AddToViewport();
			InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UInventoryComponent::AddItem(UTexture2D* ItemImage, const FString& ItemName)
{
	if (InventoryWidget)
	{
		InventoryWidget->AddItemToInventory(ItemImage, ItemName);
	}
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

void UInventoryComponent::ToggleInventory()
{
	if (!InventoryWidget) return;

	UE_LOG(LogTemp, Display, TEXT("ToggleInventory [%d"), bIsInventoryVisible);

	if (bIsInventoryVisible)
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
		bIsInventoryVisible = false;
	}
	else
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Visible);
		bIsInventoryVisible = true;
	}
}

