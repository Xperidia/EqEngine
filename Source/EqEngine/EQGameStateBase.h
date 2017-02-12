// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameStateBase.h"
#include "EQGameStateBase.generated.h"

/**
 * Connected = 0
 * InGame = 1
 * Offline = 2
 */
UCLASS()
class EQENGINE_API AEQGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	AEQGameStateBase();

	UFUNCTION(BlueprintCallable, Category = "Player State")
		int32 getCurrentPlayerState();
	
	UFUNCTION(BlueprintCallable, Category = "Player State")
		void setCurrentPlayerState(int32 NewState);

	UFUNCTION(BlueprintCallable, Category = "Credits")
		int32 GetCredits();

	UFUNCTION(BlueprintCallable, Category = "Credits")
		void GiveCredits(int32 Amount);
	
	UFUNCTION(BlueprintCallable, Category = "Credits")
		void RemoveCredits(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Credits")
		void SetCredits(int32 Amount);

private:
	UPROPERTY(EditAnywhere)
		int32 CurrentPlayerState;

	UPROPERTY(EditAnywhere)
		int32 Credits;
	
	
};
