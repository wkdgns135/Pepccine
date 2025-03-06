#include "PepccinePlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Character/Widget/MenuWidget.h"
#include "Character/Components/CrosshairHUDComponent.h"
#include "Character/Player/PepCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Character/Widget/MouseSensitivityWidget.h"

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

  MouseSensitivityClass = nullptr;
  MouseSensitivityInstance = nullptr;
  MenuInstance = nullptr;
  MenuClass = nullptr;
}

void APepccinePlayerController::BeginPlay()
{
	Super::BeginPlay();

  AddMappingContext();
  SetMenu();
  SetWidget();
}

void APepccinePlayerController::SetWidget()
{
  if (!MouseSensitivityClass) return;

  if (!MouseSensitivityInstance && MouseSensitivityClass)
  {
    MouseSensitivityInstance = CreateWidget<UMouseSensitivityWidget>(this, MouseSensitivityClass);
  }

  MouseSensitivityInstance->MouseBackButton->OnClicked.AddDynamic(this, &APepccinePlayerController::OnMouseBackButtonClicked);
  MouseSensitivityInstance->AddToViewport();
  MouseSensitivityInstance->SetVisibility(ESlateVisibility::Hidden);
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

  if (UButton* SettingButton = MenuInstance->SettingButton)
  {
    SettingButton->OnClicked.AddDynamic(this, &APepccinePlayerController::OnSettingButtonClicked);
  }

  if (UImage* GameOver = MenuInstance->GameOver)
  {
    GameOver->SetVisibility(ESlateVisibility::Hidden);
  }
}

void APepccinePlayerController::ShowGameOver(bool IsVisible)
{
  UImage* GameOver = MenuInstance->GameOver;
  UButton* BackButton = MenuInstance->BackButton;
  
  if (GameOver && BackButton)
  {
    if (IsVisible)
    {
      GameOver->SetVisibility(ESlateVisibility::Visible);
      BackButton->SetVisibility(ESlateVisibility::Hidden);
    }
    else
    {
      GameOver->SetVisibility(ESlateVisibility::Hidden);
      BackButton->SetVisibility(ESlateVisibility::Visible);
    }
  }
}

void APepccinePlayerController::OnMouseBackButtonClicked()
{
  MouseSensitivityInstance->SetVisibility(ESlateVisibility::Hidden);
  MenuInstance->SetVisibility(ESlateVisibility::Visible);
}

void APepccinePlayerController::OnBackButtonClicked()
{
  if (!MouseSensitivityInstance || !MenuInstance) return;
  
  MouseSensitivityInstance->SetVisibility(ESlateVisibility::Hidden);
  MenuInstance->SetVisibility(ESlateVisibility::Hidden);
  bShowMouseCursor = false;
  SetInputMode(FInputModeGameOnly());

  if (APepCharacter* User = Cast<APepCharacter>(GetCharacter()))
  {
    User->PauseGame(false);
  }
}

void APepccinePlayerController::OnExitButtonClicked()
{
  UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}

void APepccinePlayerController::OnSettingButtonClicked()
{
  if (!MouseSensitivityInstance || !MenuInstance) return;

  UE_LOG(LogTemp, Display, TEXT("OnSettingButtonClicked"));
  
  MenuInstance->SetVisibility(ESlateVisibility::Hidden);
  MouseSensitivityInstance->SetVisibility(ESlateVisibility::Visible);
  bShowMouseCursor = true;
  SetInputMode(FInputModeUIOnly());
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