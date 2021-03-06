// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "EqEngineCharacter.generated.h"

class UInputComponent;
class AEQUsableActor;
class ASBase;

UENUM()
enum EPlayerTask
{
	Idle,
	Fire,
	Spell
};

UCLASS(config=Game)
class AEqEngineCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	class USkeletalMeshComponent* Mesh1P;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* FP_Gun;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* FP_MuzzleLocation;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(ReplicatedUsing=OnRep_PlayerTask)
		TEnumAsByte<EPlayerTask> PlayerTask;

	FTimerHandle TimerHandler_PlyTask;
	FTimerHandle TimerHandler_PlyRegen;

public:
	AEqEngineCharacter();

	virtual void BeginPlay();
	virtual void Tick(float DeltaTime);

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector GunOffset;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class AEqEngineProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class ASBase> SpellProjectileClass;


	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	class USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* FireAnimation;

public:
	UFUNCTION(BlueprintCallable, Category = "Player Properties")
		float getHealth() const;


	UFUNCTION(BlueprintCallable, Category = "Player Properties")
		float getStamina() const;

	UFUNCTION(BlueprintCallable, Category = "Player Properties")
		FString getPlayerName() const;

	UFUNCTION(BlueprintCallable, Category = "Player Properties")
		void setPlayerName(FString NewPlayerName);

	UFUNCTION()
		void OnRep_Health();

	UFUNCTION()
		void OnRep_PlayerTask();

	UFUNCTION()
		void OnRep_Stamina();

	UFUNCTION()
		void OnRep_PlayerName();

	void StartFiring();
	void StopFiring();

	void StartSpell();
	void StopSpell();
	
	void OnShootSpell();

	void StartSprinting();
	void StopSprinting();

	
	void Regen();

	void ExecuteTask(EPlayerTask Task);

	UFUNCTION(Server, Reliable, WithValidation)
		void SV_ExecuteTask(EPlayerTask Task);

	void PlayerIsSprinting(bool isSprinting);

	UFUNCTION(Server, Reliable, WithValidation)
		void SV_PlayerIsSprinting(bool isSprinting);

	void SetPlayerWalkSpeed(int32 speed);

	UFUNCTION(Server, Reliable, WithValidation)
		void SV_SetPlayerWalkSpeed(int32 speed);

	FRotator GetViewRotation() const override;
public:
	UPROPERTY(EditDefaultsOnly, Category = "Player Properties")
		float MaxHealth = 100.0f;
private:
	// Replicated
	UPROPERTY(EditDefaultsOnly, Replicated, ReplicatedUsing = OnRep_Health, Category = "Player Properties")
		float Health;

	UPROPERTY(EditDefaultsOnly, Replicated, ReplicatedUsing = OnRep_Stamina, Category = "Player Properties")
		float Stamina;

	UPROPERTY(EditDefaultsOnly, Category = "Player Properties")
		bool bIsSprinting;

	UPROPERTY(EditDefaultsOnly, Replicated, ReplicatedUsing = OnRep_PlayerName, Category = "Player Properties")
		FString PlayerName;

protected:
	
	/** Fires a projectile. */
	void OnFire();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser);
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

