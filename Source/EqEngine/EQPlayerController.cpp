// Fill out your copyright notice in the Description page of Project Settings.

#include "EqEngine.h"
#include "EQPlayerController.h"

/** Lorsque le joueur meurt, cette fonction est appelé par le character */
void AEQPlayerController::OnDeath()
{
	UnPossess();
	GetWorldTimerManager().SetTimer(TH_Respawn, this, &AEQPlayerController::Respawn, 3.0f);
}

/** Respawn le joueu */
void AEQPlayerController::Respawn()
{
	AGameModeBase * GM = GetWorld()->GetAuthGameMode();

	if (GM)
	{
		APawn * NP = GM->SpawnDefaultPawnFor(this, StartSpot.Get());
		Possess(NP);
	}
}
