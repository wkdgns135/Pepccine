#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UInventoryItemWidget;
class UUniformGridPanel;
class UImage;
class UScrollBox;

UCLASS()
class PEPCCINE_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void AddItemToInventory(UTexture2D* ItemImage, const FString& ItemName);

protected:
	UPROPERTY(meta = (BindWidget))
	UUniformGridPanel* InventoryGrid;

	UPROPERTY(meta = (BindWidget))
	UImage* BackgroundImage;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* InventoryScrollBox;
	
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UInventoryItemWidget> ItemWidgetClass;
	
private:
	int32 CurrentRow = 0;
	int32 CurrentColumn = 0;

	const int BoxWidthSize = 180;
	const int BoxHeightSize = 180;

	const int32 MaxColumns = 7;
	const int32 MaxRows = 7;

	void SetEmptyGrid();
	void CreateNewGridBlock();
};
