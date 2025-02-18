// Copyright Epic Games, Inc. All Rights Reserved.

#include "PepccineGameMode.h"
#include "PepccineCharacter.h"
#include "PepccineGameState.h"
#include "UObject/ConstructorHelpers.h"

APepccineGameMode::APepccineGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	GameStateClass = APepccineGameState::StaticClass();
}
