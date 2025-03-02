#include "MenuWidget.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/PlayerController.h"
#include "Character/Controller/PepccinePlayerController.h"

bool UMenuWidget::Initialize()
{
	Super::Initialize();

	if (ExitButton)
	{
		ExitButton->OnClicked.AddDynamic(this, &UMenuWidget::OnExitButtonClicked);
	}

	if (BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &UMenuWidget::OnBackButtonClicked);
	}

	return true;
}

void UMenuWidget::OnExitButtonClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}

void UMenuWidget::OnBackButtonClicked()
{
	RemoveFromParent();
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	
	if (APepccinePlayerController* PepController = Cast<APepccinePlayerController>(PlayerController))
	{
		PepController->SetShowMouseCursor(false);
	}
}
