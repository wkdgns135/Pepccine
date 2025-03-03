#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MonsterHealthWidget.generated.h"

UCLASS()
class PEPCCINE_API UMonsterHealthWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Health")
	void UpdateHealthBar(float HealthPercent);

protected:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthProgressBar;
};
