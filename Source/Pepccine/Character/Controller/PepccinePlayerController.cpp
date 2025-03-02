#include "PepccinePlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Character/Widget/MenuWidget.h"
#include "Character/Components/CrosshairHUDComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/Button.h"

APepccinePlayerController::APepccinePlayerController()
{
  InputMappingContext = nullptr;
  MoveAction = nullptr;
  JumpAction = nullptr;
  LookAction = nullptr;
  SprintAction = nullptr;
  ItemUseAction = nullptr;
  CrouchAction = nullptr;
  ReloadingAction = nullptr;
  SwapAction = nullptr;
  InteractiveAction = nullptr;
  MenuAction = nullptr;
  InventoryAction = nullptr;
  FireAction = nullptr;
  ZoomAction = nullptr;

  MenuInstance = nullptr;
  MenuClass = nullptr;
}

void APepccinePlayerController::BeginPlay()
{
	Super::BeginPlay();

  AddMappingContext();
  SetMenu();
}

void APepccinePlayerController::SetMenu()
{
  if (!MenuClass) return;

  if (!MenuInstance && MenuClass)
  {
    MenuInstance = CreateWidget<UMenuWidget>(this, MenuClass);
  }

  MenuInstance->AddToViewport();
  MenuInstance->SetVisibility(ESlateVisibility::Hidden);

  if (UButton* ExitButton = MenuInstance->ExitButton)
  {
    ExitButton->OnClicked.AddDynamic(this, &APepccinePlayerController::OnExitButtonClicked);
  }

  if (UButton* BackButton = MenuInstance->BackButton)
  {
    BackButton->OnClicked.AddDynamic(this, &APepccinePlayerController::OnBackButtonClicked);
  }
}

void APepccinePlayerController::OnBackButtonClicked()
{
  MenuInstance->SetVisibility(ESlateVisibility::Hidden);
  bShowMouseCursor = false;
  SetInputMode(FInputModeGameOnly());
}

void APepccinePlayerController::OnExitButtonClicked()
{
  UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}

void APepccinePlayerController::ToggleExitMenu()
{
  if (ESlateVisibility::Hidden == MenuInstance->GetVisibility())
  {
    MenuInstance->SetVisibility(ESlateVisibility::Visible);
    bShowMouseCursor = true;
    SetInputMode(FInputModeUIOnly());
  }
}

void APepccinePlayerController::AddMappingContext()
{
  if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
  {
    if (UEnhancedInputLocalPlayerSubsystem* SubSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
    {
      if (InputMappingContext)
      {
        SubSystem->AddMappingContext(InputMappingContext, 0);
      }
    }
  }
}

void APepccinePlayerController::SetupInputComponent()
{
  UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
  if (!EnhancedInputComponent) return;
}