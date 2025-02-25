#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UInventoryItemWidget;
class UUniformGridPanel;
class UImage;

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

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UInventoryItemWidget> ItemWidgetClass;

	UPROPERTY(meta=(BindWidget))
	UImage* BackgroundImage;
private:
	int32 CurrentRow = 0;
	int32 CurrentColumn = 0;
	
};
