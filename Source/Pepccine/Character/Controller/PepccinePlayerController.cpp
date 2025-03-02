#include "PepccinePlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Character/Widget/MenuWidget.h"
#include "Character/Components/CrosshairHUDComponent.h"

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
}

void APepccinePlayerController::BeginPlay()
{
	Super::BeginPlay();

  AddMappingContext();
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

void APepccinePlayerController::ToggleExitMenu()
{
  if (!MenuInstance && MenuClass)
  {
    MenuInstance = CreateWidget<UMenuWidget>(this, MenuClass);
  }

  if (MenuInstance && !MenuInstance->IsInViewport())
  {
    MenuInstance->AddToViewport();
    bShowMouseCursor = true;
    SetInputMode(FInputModeUIOnly());
  }
  else
  {
    MenuInstance->RemoveFromParent();
    bShowMouseCursor = false;
    SetInputMode(FInputModeGameOnly());
  }
}