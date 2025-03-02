#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.generated.h"

UCLASS()
class PEPCCINE_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;

private:
	UFUNCTION()
	void OnExitButtonClicked();

	UFUNCTION()
	void OnBackButtonClicked();

	UPROPERTY(meta = (BindWidget))
	class UButton* ExitButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* BackButton;
};
