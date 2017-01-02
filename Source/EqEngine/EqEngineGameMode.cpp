// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "EqEngine.h"
#include "EqEngineGameMode.h"
#include "EqEngineHUD.h"
#include "EqEngineCharacter.h"

AEqEngineGameMode::AEqEngineGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AEqEngineHUD::StaticClass();
}
