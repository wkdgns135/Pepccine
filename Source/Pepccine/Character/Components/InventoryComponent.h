#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class UInventoryWidget;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PEPCCINE_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable)
	void AddItem(UTexture2D* ItemImage, const FString& ItemName, const FString& ItemDetail, const FString& StatDetail);

	UFUNCTION(BlueprintCallable)
	void RemoveAllItem();

	UFUNCTION(BlueprintCallable)
	void ToggleInventory();
protected:
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;

	UPROPERTY()
	UInventoryWidget* InventoryWidget;

	bool bIsInventoryVisible = false;
};
