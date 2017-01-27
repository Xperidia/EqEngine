// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "EqEngine.h"
#include "EqEngineGameMode.h"
#include "EqEngineHUD.h"
#include "EqEngineCharacter.h"
#include "EQPlayerController.h"
#include "EQGameStateBase.h"
#include "EQPlayerState.h"

AEqEngineGameMode::AEqEngineGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AEqEngineHUD::StaticClass();

	PlayerControllerClass = AEQPlayerController::StaticClass();
	GameStateClass = AEQGameStateBase::StaticClass();
	PlayerStateClass = AEQPlayerState::StaticClass();
}
