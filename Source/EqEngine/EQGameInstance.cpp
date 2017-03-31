// Fill out your copyright notice in the Description page of Project Settings.

#include "EqEngine.h"
#include "EQGameInstance.h"

/*
Inutile
void UEQGameInstance::ShowMainMenu() 
{
	if (MainMenuWidgetRef != nullptr) return;

	if (MainMenuWidget == nullptr)
	{
		MainMenuWidget = CreateWidget<UUserWidget>(this, MainMenuWidgetRef);
		MainMenuWidget->AddToViewport();
	}
	else
	{
		MainMenuWidget->AddToViewport();
	}

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = true;
}
*/