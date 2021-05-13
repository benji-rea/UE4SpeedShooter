// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"

#include "Projectile.h"
#include "FPSWeapon.h"
#include "SimpleAmmo.h"

#include "SpeedShooterCharacter.generated.h"


UCLASS()
class SPEEDSHOOTER_API ASpeedShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Constructor class.
	ASpeedShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Projectile class to spawn.
	UPROPERTY(BlueprintReadWrite, Category = Projectile)
	TSubclassOf<class AProjectile> ProjectileClass;

public:

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Handles input for moving forward and backward.
	UFUNCTION()
	void MoveLatitude(float Value);

	// Handles input for moving right and left.
	UFUNCTION()
	void MoveLongitude(float Value);

	// Initialise jump flag on key press.
	UFUNCTION()
	void StartJump();

	// Destroys jump flag on key release.
	UFUNCTION()
	void StopJump();

	// Function that handles firing projectiles.
	UFUNCTION(BlueprintCallable)
	void Fire();

	// Init the sprint flag.
	UFUNCTION()
	void StartSprint();
	// Destroys the sprint flag.
	UFUNCTION()
	void StopSprint();

	// Init the zoom  flag.
	UFUNCTION()
	void StartZoom();

	// Destroys the zoom flag.
	UFUNCTION()
	void StopZoom();

	// Use Ability function
	void UseAbility1();
	void UseAbility2();

	// Reset to default

	void ResetAbility1();

	void ResetAbility2();

	// Allow the ability to be used again.

	void AbilityCooldownComplete1();
	void AbilityCooldownComplete2();

	// Damage Handler
	UFUNCTION(BlueprintCallable)
	void DamageHandler(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	// Respawn Manager
	UFUNCTION(BlueprintCallable)
	void Die();

	UFUNCTION(BlueprintCallable)
	void Respawn();

	// Reload current weapon.
	UFUNCTION(BlueprintCallable)
	//void ReloadWeapon();
	void ReloadWeapon(EWeaponType _weaponType);

	// Reload current weapon when button pressed
	void ManualReloadWeapon();

	// Ammo Calculation for clip + inventory
	int AmmoCalc(int _ammoCount);
	
	// Switch the current weapon to the next in change
	void SwitchWeapon();

	// Weapon Switch mesh handler
	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void SwitchWeaponMesh(int _index);

	// Alert the player that there is no ammo left. (To access widget on the hud without creating another code to blueprint dependency
	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void OutOfAmmoPopUp();

	// Increase ammo to the correct ammo type
	UFUNCTION(BlueprintCallable)
	void AddAmmo(EAmmoType _ammoType, int _ammoCount);


	// Camera management.
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent;

	// Arm mesh for first person camera.
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* FPSMesh;

	// Bullet offset from camera.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;

// VARIABLES
	
	// Sprint flag.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	bool isSprinting;

	// Zoom flag.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	bool isZoomed;

	// Ability Use flag.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
	bool hasUsedAbility1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
	bool hasUsedAbility2;

	// Ability Durations.	
	float abilityDuration1;
	float abilityDuration2;

	// Ability Cooldown
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
	float abilityCooldown1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
	float abilityCooldown2;

	// Health flag
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
	float Health;

	// Respawn Location Flag
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Respawn)
	FVector respawnLoc;

	// Timer handles.
	FTimerHandle abilityTimerHandler1;
	FTimerHandle abilityTimerHandler2;

	// Weapon Handlers
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	int WeaponIndex;

	// Pointer setup
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TArray<AFPSWeapon*> Weapons;

	// Ammo Flags
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	int rifleAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	int pistolAmmo;
};