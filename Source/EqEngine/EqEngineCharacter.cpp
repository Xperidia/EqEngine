// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "EqEngine.h"
#include "EqEngineCharacter.h"
#include "EqEngineProjectile.h"
#include "EQUsableActor.h"
#include "EQFuncs.h"
#include "EQPlayerController.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "MotionControllerComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AEqEngineCharacter

AEqEngineCharacter::AEqEngineCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Gun->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.

	Health = MaxHealth;

	Stamina = 100.0f;
}

void AEqEngineCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	Mesh1P->SetHiddenInGame(false, true);
}

void AEqEngineCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FirstPersonCameraComponent->SetWorldRotation(GetViewRotation());

	if (Stamina <= 100.0f && !bIsSprinting)
	{
		Stamina += 0.5f;
		OnRep_Stamina();
	}

	if (Stamina > 0.0f && bIsSprinting)
	{
		Stamina -= 0.5f;
		OnRep_Stamina();
	}

	if (Stamina == 0.0f)
	{
		bIsSprinting = false;
		GetCharacterMovement()->MaxWalkSpeed = 600;
	}
}

void AEqEngineCharacter::PlayerIsSprinting(bool isSprinting)
{
	if (GetNetMode() == NM_Client)
	{
		SV_PlayerIsSprinting(isSprinting);
		return;
	}

	bIsSprinting = isSprinting;
}

void AEqEngineCharacter::SetPlayerWalkSpeed(int32 speed)
{
	if (GetNetMode() == NM_Client)
	{
		SV_SetPlayerWalkSpeed(speed);
		return;
	}

	GetCharacterMovement()->MaxWalkSpeed = speed;
}

void AEqEngineCharacter::SV_PlayerIsSprinting_Implementation(bool isSprinting)
{
	bIsSprinting = isSprinting;
}

bool AEqEngineCharacter::SV_PlayerIsSprinting_Validate(bool isSprinting)
{
	return true;
}

void AEqEngineCharacter::SV_SetPlayerWalkSpeed_Implementation(int32 speed)
{
	GetCharacterMovement()->MaxWalkSpeed = speed;
}

bool AEqEngineCharacter::SV_SetPlayerWalkSpeed_Validate(int32 speed)
{
	return true;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AEqEngineCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AEqEngineCharacter::StartFiring);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AEqEngineCharacter::StopFiring);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AEqEngineCharacter::StartSprinting);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AEqEngineCharacter::StopSprinting);

	//PlayerInputComponent->BindAction("Use", IE_Pressed, this, &AEqEngineCharacter::Use);

	PlayerInputComponent->BindAxis("MoveForward", this, &AEqEngineCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AEqEngineCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AEqEngineCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AEqEngineCharacter::LookUpAtRate);
}

float AEqEngineCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser)
{
	float Dmg = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Health -= Dmg;

	if (Health <= 0)
	{
		AEQPlayerController * PlayerController = Cast<AEQPlayerController>(Controller);
	
		if (PlayerController)
		{
			PlayerController->OnDeath();
		}

		Destroy();
	}

	OnRep_Health();

	return Dmg;
}

void AEqEngineCharacter::OnRep_Health()
{
	FirstPersonCameraComponent->PostProcessSettings.SceneFringeIntensity = 5.0f - Health * 0.05f;
}

void AEqEngineCharacter::OnRep_PlayerTask()
{
	if (PlayerTask == EPlayerTask::Idle)
	{
		// nothing
	}
	else if (PlayerTask == EPlayerTask::Fire)
	{
		OnFire();
	}
}

void AEqEngineCharacter::OnRep_Stamina()
{

}

void AEqEngineCharacter::StartSprinting()
{
	if (Stamina > 0.0f)
	{
		PlayerIsSprinting(true);
		SetPlayerWalkSpeed(1200);
	}
}

void AEqEngineCharacter::StopSprinting()
{
	PlayerIsSprinting(false);
	SetPlayerWalkSpeed(600);
}

void AEqEngineCharacter::StartFiring()
{
	ExecuteTask(EPlayerTask::Fire);
}

void AEqEngineCharacter::StopFiring()
{
	ExecuteTask(EPlayerTask::Idle);
}

FRotator AEqEngineCharacter::GetViewRotation() const
{
	if (Controller)
	{
		return Controller->GetControlRotation();
	}

	return FRotator(RemoteViewPitch / 255.0f * 360.0f, GetActorRotation().Yaw, 0.0f);
}

void AEqEngineCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEqEngineCharacter, Health);
	DOREPLIFETIME(AEqEngineCharacter, PlayerTask);
	DOREPLIFETIME(AEqEngineCharacter, Stamina);
}

void AEqEngineCharacter::ExecuteTask(EPlayerTask Task)
{
	if (GetNetMode() == NM_Client)
	{
		SV_ExecuteTask(Task);
		return;
	}

	PlayerTask = Task;
	OnRep_PlayerTask();
}

void AEqEngineCharacter::SV_ExecuteTask_Implementation(EPlayerTask Task)
{
	ExecuteTask(Task);
}

bool AEqEngineCharacter::SV_ExecuteTask_Validate(EPlayerTask Task)
{
	return true;
}

void AEqEngineCharacter::OnFire()
{
	if (PlayerTask != EPlayerTask::Fire) return;
	// try and fire a projectile
	if (ProjectileClass != NULL)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			const FRotator SpawnRotation = GetControlRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// spawn the projectile at the muzzle
			World->SpawnActor<AEqEngineProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		}
	}

	// try and play the sound if specified
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (FireAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}

	GetWorldTimerManager().SetTimer(TimerHandler_PlyTask, this, &AEqEngineCharacter::OnFire, 0.1f);
}

void AEqEngineCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AEqEngineCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AEqEngineCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AEqEngineCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

float AEqEngineCharacter::getHealth() const
{
	return Health;
}

float AEqEngineCharacter::getStamina() const
{
	return Stamina;
}