// Fill out your copyright notice in the Description page of Project Settings.

#include "EqEngine.h"
#include "SBase.h"


// Sets default values
ASBase::ASBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASBase::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

