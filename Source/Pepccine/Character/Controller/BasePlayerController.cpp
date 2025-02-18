// Fill out your copyright notice in the Description page of Project Settings.


#include "Pepccine/Character/Controller/BasePlayerController.h"
#include "EnhancedInputSubsystems.h"

ABasePlayerController::ABasePlayerController()
{

}

void ABasePlayerController::BeginPlay()
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