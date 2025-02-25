#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryItemWidget.generated.h"

class UButton;
class UTextBlock;
class UImage;

UCLASS()
class PEPCCINE_API UInventoryItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void SetItem(UTexture2D* ItemImage, const FString& ItemName);

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* ItemButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TooltipInfo;

	UPROPERTY(meta = (BindWidget))
	UImage* ItemImageWidget;

private:
	UFUNCTION()
	void OnHovered();

	UFUNCTION()
	void OnUnhovered();
};
