// Fill out your copyright notice in the Description page of Project Settings.


#include "PepccinePlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

APepccinePlayerController::APepccinePlayerController()
{
  InputMappingContext = nullptr;
  MoveAction = nullptr;
}

void APepccinePlayerController::BeginPlay()
{
	Super::BeginPlay();

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

  // BindAction(const UInputAction* Action, ETriggerEvent TriggerEvent, UObject* Object, FName FunctionName)
  EnhancedInputComponent->BindAction(MenuAction, ETriggerEvent::Started, this, &APepccinePlayerController::OpenMenu);
}

void APepccinePlayerController::OpenMenu()
{
  UE_LOG(LogTemp, Log, TEXT("Menu Opened!"));
}