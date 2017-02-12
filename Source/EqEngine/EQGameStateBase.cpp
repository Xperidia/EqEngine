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


int32 AEQGameStateBase::GetCredits()
{
	return Credits;
}

void AEQGameStateBase::GiveCredits(int32 Amount)
{
	Credits += Amount;
}

void AEQGameStateBase::RemoveCredits(int32 Amount)
{
	Credits -= Amount;
}

void AEQGameStateBase::SetCredits(int32 Amount)
{
	Credits = Amount;
}