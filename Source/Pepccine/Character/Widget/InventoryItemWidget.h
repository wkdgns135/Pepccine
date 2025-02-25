// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryItemWidget.generated.h"

class UButton;
class UTextBlock;

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

private:
	UFUNCTION()
	void OnHovered();

	UFUNCTION()
	void OnUnhovered();
};
