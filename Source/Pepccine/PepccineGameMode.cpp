// Copyright Epic Games, Inc. All Rights Reserved.

#include "PepccineGameMode.h"
#include "PepccineCharacter.h"
#include "PepccineGameState.h"
#include "UObject/ConstructorHelpers.h"

APepccineGameMode::APepccineGameMode()
{
	GameStateClass = APepccineGameState::StaticClass();
}
