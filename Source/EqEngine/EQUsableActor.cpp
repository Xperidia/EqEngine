// Fill out your copyright notice in the Description page of Project Settings.

#include "EqEngine.h"
#include "EQUsableActor.h"


// Sets default values
AEQUsableActor::AEQUsableActor()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEQUsableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEQUsableActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

