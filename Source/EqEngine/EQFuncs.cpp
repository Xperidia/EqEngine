// Fill out your copyright notice in the Description page of Project Settings.

#include "EqEngine.h"
#include "EQFuncs.h"

template<class T> T UEQFuncs::GetActorLineSingleByChannel(FVector StartTrace, FRotator Rotator, float RaycastMaxDistance)
{
	const FVector Direction = Rotator.Vector();
	const FVector TraceEnd = StartTrace + (Direction * RaycastMaxDistance);

	FCollisionQueryParams TraceParams(FName(TEXT("TP")), true);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = false;
	TraceParams.bTraceComplex = true;

	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, TraceEnd, ECC_Visibility, TraceParams);

	if (Cast<T>(Hit.GetActor()))
	{
		T returnActor = Cast<T>(Hit.GetActor());

		return returnActor;
	}
	else
	{
		return nullptr;
	}
}

float UEQFuncs::GetActorDistanceLineSingleByChannel(FVector StartTrace, FRotator Rotator, float RaycastMaxDistance)
{
	const FVector Direction = Rotator.Vector();
	const FVector TraceEnd = StartTrace + (Direction * RaycastMaxDistance);

	FCollisionQueryParams TraceParams(FName(TEXT("TP")), true);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = false;
	TraceParams.bTraceComplex = true;

	FHitResult Hit(ForceInit);
	GEngine->GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, TraceEnd, ECC_Visibility, TraceParams);

	float returnDistance = Hit.Distance;

	return returnDistance;
}