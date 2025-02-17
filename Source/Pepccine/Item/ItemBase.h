#pragma once

#include "CoreMinimal.h"
#include "ItemBase.generated.h"

UCLASS(BlueprintType, Blueprintable)
class PEPCCINE_API UItemBase : public UObject
{
	GENERATED_BODY()
	
public:	
	UItemBase();

	UFUNCTION()
	FName GetItemName() const;
	UFUNCTION()
	FString GetItemDescription() const;
	UFUNCTION()
	int32 GetItemSellingPrice() const;


protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item");
	FName Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item");
	FString Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item");
	int32 SellingPrice;
};
