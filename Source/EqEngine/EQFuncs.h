// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EQFuncs.generated.h"

UCLASS()
class EQENGINE_API UEQFuncs : public UObject
{
	GENERATED_BODY()

public:
	template<class T> static T GetActorLineSingleByChannel(FVector StartTrace, FRotator Rotator, float RaycastMaxDistance);
	static float GetActorDistanceLineSingleByChannel(FVector StartTrace, FRotator Rotator, float RaycastMaxDistance);
};
