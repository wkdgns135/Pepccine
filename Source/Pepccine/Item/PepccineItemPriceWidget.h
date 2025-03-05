#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "PepccineItemPriceWidget.generated.h"

class UTextBlock;

UCLASS()
class PEPCCINE_API UPepccineItemPriceWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PriceText;
};
