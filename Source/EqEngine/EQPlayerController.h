// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "EQPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class EQENGINE_API AEQPlayerController : public APlayerController
{
	GENERATED_BODY()

		FTimerHandle TH_Respawn;

public:
	void OnDeath();
	void Respawn();
};
