#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryItemWidget.generated.h"

class UButton;
class UTextBlock;
class UImage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemHovered, int32, ItemIndex, bool, bIsHovered);

UCLASS()
class PEPCCINE_API UInventoryItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	int32 GetItemIndex() const { return ItemIndex; }

	bool bIsEmpty = true;

	void SetItem(UTexture2D* ItemImage, const FString& ItemName, int32 Index);
	void SetEmpty();

	UPROPERTY(BlueprintAssignable, Category = "Item")
	FOnItemHovered OnItemHovered;
protected:
	UPROPERTY(meta = (BindWidget))
	UButton* ItemButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TooltipInfo;

	UPROPERTY(meta = (BindWidget))
	UImage* ItemImageWidget;

private:
	int32 ItemIndex;
	
	UFUNCTION()
	void OnHovered();

	UFUNCTION()
	void OnUnhovered();
};
