// Fill out your copyright notice in the Description page of Project Settings.

#include "EqEngine.h"
#include "EQGameStateBase.h"

AEQGameStateBase::AEQGameStateBase()
{
	CurrentPlayerState = 2; // 2 = Offline, 1 = InGame, 0 = Connected
}

int32 AEQGameStateBase::getCurrentPlayerState()
{
	return CurrentPlayerState;
}

void AEQGameStateBase::setCurrentPlayerState(int32 NewState)
{
	if (NewState > 2 || NewState < 0)
	{
		CurrentPlayerState = 0;
	}

	CurrentPlayerState = NewState;
}
