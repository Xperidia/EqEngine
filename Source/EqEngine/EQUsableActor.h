// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "EQUsableActor.generated.h"

UCLASS()
class EQENGINE_API AEQUsableActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEQUsableActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
	
	
};
