// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once 
#include "GameFramework/HUD.h"
#include "EqEngineHUD.generated.h"

UCLASS()
class AEqEngineHUD : public AHUD
{
	GENERATED_BODY()

public:
	AEqEngineHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

