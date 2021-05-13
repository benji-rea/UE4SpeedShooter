// Fill out your copyright notice in the Description page of Project Settings.


#include "SpeedShooterCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"

// CONSTRUCTOR
ASpeedShooterCharacter::ASpeedShooterCharacter()
{
	// Initialise camera component.
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));

	// Attach camera component to capsule.
	CameraComponent->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));

	// Sets pos slightly above the eyes.
	CameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));

	// Link pawn rotation to camera rotation
	CameraComponent->bUsePawnControlRotation = true;

	// Initialise mesh.
	FPSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));

	// Set visible to owner.
	FPSMesh->SetOnlyOwnerSee(true);

	// Attach mesh to camera.
	FPSMesh->SetupAttachment(CameraComponent);

	// Disable shadows on mesh to prevent double shadowing
	FPSMesh->bCastDynamicShadow = false;
	FPSMesh->CastShadow = false;

	// Disable the third person mesh for owner.
	GetMesh()->SetOwnerNoSee(true);

	isSprinting = false;
	isZoomed = false;

	//Weapon = nullptr;

	//Ability constructor.
	hasUsedAbility1 = false;
	hasUsedAbility2 = false;
	abilityDuration1 = 4.0f;
	abilityDuration2 = 6.0f;
	abilityCooldown1 = 4.0f;
	abilityCooldown2 = 8.0f;

	Health = 1.0f;

	WeaponIndex = 0;

	rifleAmmo = 60;
	pistolAmmo = 30;

	// REPLACE WITH INSTANCE REFERENCE
	respawnLoc = FVector(160.000000,-170.000000,20.100000);
}

// Called when the game starts or when spawned
void ASpeedShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Candidate profile loaded."));

	OnTakeAnyDamage.AddDynamic(this, &ASpeedShooterCharacter::DamageHandler);

}

// Called to bind functionality to input
void ASpeedShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up movement axis.
	PlayerInputComponent->BindAxis("MoveLatitude", this, &ASpeedShooterCharacter::MoveLatitude);
	PlayerInputComponent->BindAxis("MoveLongitude", this, &ASpeedShooterCharacter::MoveLongitude);

	// Set up look axis.
	PlayerInputComponent->BindAxis("LookLatitude", this, &ASpeedShooterCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookLongitude", this, &ASpeedShooterCharacter::AddControllerPitchInput);

	// Set up jump.
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASpeedShooterCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ASpeedShooterCharacter::StopJump);

	// Set up shooting & reloading.
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASpeedShooterCharacter::Fire);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ASpeedShooterCharacter::ManualReloadWeapon);
	
	// Set up sprint.
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASpeedShooterCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASpeedShooterCharacter::StopSprint);

	// Set up zoom.
	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &ASpeedShooterCharacter::StartZoom);
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &ASpeedShooterCharacter::StopZoom);

	// Set up abilities.
	PlayerInputComponent->BindAction("Ability1", IE_Pressed, this, &ASpeedShooterCharacter::UseAbility1);
	PlayerInputComponent->BindAction("Ability2", IE_Pressed, this, &ASpeedShooterCharacter::UseAbility2);

	// Set up weapon switching
	PlayerInputComponent->BindAction("SwitchWeapon", IE_Pressed, this, &ASpeedShooterCharacter::SwitchWeapon);
}

void ASpeedShooterCharacter::MoveLatitude(float Value)
{
	// Find out which way is "forward" and record that the player wants to move that way.
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void ASpeedShooterCharacter::MoveLongitude(float Value)
{
	// Find out which way is "right" and record that the player wants to move that way.
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

void ASpeedShooterCharacter::StartJump()
{
	bPressedJump = true;
}

void ASpeedShooterCharacter::StopJump()
{
	bPressedJump = false;
}

void ASpeedShooterCharacter::Fire()
{
	// Attempt to fire a projectile.
	if (ProjectileClass)
	{
		// Get the camera transform.
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);
		// Set MuzzleOffset to spawn projectiles slightly in front of the camera.
		MuzzleOffset.Set(100.0f, 0.0f, 0.0f);
		// Transform MuzzleOffset from camera space to world space.
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);
		// Skew the aim to be slightly upwards.
		FRotator MuzzleRotation = CameraRotation;
		MuzzleRotation.Pitch += 10.0f;
			UWorld* World = GetWorld();
		if (World)
		{
			if (Weapons[WeaponIndex])
			{
				if (Weapons[WeaponIndex]->clipAmmo > 0)
				{
					FActorSpawnParameters SpawnParams;
					SpawnParams.Owner = this;
					SpawnParams.Instigator = GetInstigator();
						// Spawn the projectile at the muzzle.
					AProjectile* Projectile = World->SpawnActor<AProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
					if (Projectile)
					{
						// Set the projectile's initial trajectory.
						FVector LaunchDirection = MuzzleRotation.Vector();
						Projectile->FireInDirection(LaunchDirection);
					}
						Weapons[WeaponIndex]->clipAmmo -=1;
				}
				else
				{
					// Automatically reloads if there is ammo in inventory, but clip is empty
					ReloadWeapon(Weapons[WeaponIndex]->weaponType);
				}						
			}
		}				
	}
}

void ASpeedShooterCharacter::StartSprint()
{
	if (auto characterMovement = GetCharacterMovement())
	{
		UE_LOG(LogTemp, Warning, TEXT("Sprint Activated"));
		characterMovement->MaxWalkSpeed = 1500.0f;
		isSprinting = true;
	}
}

void ASpeedShooterCharacter::StopSprint()
{
	if (auto characterMovement = GetCharacterMovement())
	{
		UE_LOG(LogTemp, Warning, TEXT("Sprint Deactivated"));
		characterMovement->MaxWalkSpeed = 600.0f;
		isSprinting = false;
	}
}

void ASpeedShooterCharacter::StartZoom()
{
	if (auto firstPersonCamera = CameraComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Zoom Activated"));
		firstPersonCamera->SetFieldOfView(70.0f);
		isZoomed = true;
	}
}

void ASpeedShooterCharacter::StopZoom()
{
	if (auto firstPersonCamera = CameraComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Zoom Deactivated"));
		firstPersonCamera->SetFieldOfView(90.0f);
		isZoomed = false;
	}
}

void ASpeedShooterCharacter::UseAbility1()
{
	UE_LOG(LogTemp, Warning, TEXT("Ability 1 used."));
	if (!hasUsedAbility1)
	{
		hasUsedAbility1 = true;
		JumpMaxCount = 3;

		GetWorld()->GetTimerManager().SetTimer(abilityTimerHandler1, this, &ASpeedShooterCharacter::ResetAbility1, abilityDuration1, false);
	}
}

void ASpeedShooterCharacter::UseAbility2()
{
	UE_LOG(LogTemp, Warning, TEXT("Ability 2 used."));
	if (!hasUsedAbility2)
	{
		hasUsedAbility2 = true;
        if (auto characterMovement = GetCharacterMovement())
        {
	        characterMovement->MaxWalkSpeed = 5000;
        }

		GetWorld()->GetTimerManager().SetTimer(abilityTimerHandler2, this, &ASpeedShooterCharacter::ResetAbility2, abilityDuration2, false);
	}
}

void ASpeedShooterCharacter::ResetAbility1()
{
	JumpMaxCount = 1;
	GetWorld()->GetTimerManager().SetTimer(abilityTimerHandler1, this, &ASpeedShooterCharacter::AbilityCooldownComplete1, abilityCooldown1, false);
}

void ASpeedShooterCharacter::ResetAbility2()
{
	if (auto characterMovement = GetCharacterMovement())
	{
		characterMovement->MaxWalkSpeed = 600;
	}
	GetWorld()->GetTimerManager().SetTimer(abilityTimerHandler2, this, &ASpeedShooterCharacter::AbilityCooldownComplete2, abilityCooldown2, false);
}

void ASpeedShooterCharacter::AbilityCooldownComplete1()
{
	hasUsedAbility1 = false;
}

void ASpeedShooterCharacter::AbilityCooldownComplete2()
{
	hasUsedAbility2 = false;
}

void ASpeedShooterCharacter::DamageHandler(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	// Deal Damage to player
	Health -= Damage;

	// Logic fix to prevent negative health, would use a floor in C#? Also calls Death
	if (Health < 0.0f)
	{
		Health = 0.0f;
		Die();
	}
}


void ASpeedShooterCharacter::Die()
{
	Respawn();
	// More Death management to add later, Animation + Sound
}

void ASpeedShooterCharacter::Respawn()
{
	// Reset Health and spawn actor at start
	Health = 1.0f;
	SetActorLocation(respawnLoc);
}

void ASpeedShooterCharacter::ReloadWeapon(EWeaponType _weaponType)
{
	if(Weapons[WeaponIndex])
	{
		switch(_weaponType)
		{
		case EWeaponType::E_Rifle:
			rifleAmmo = AmmoCalc(rifleAmmo);
			break;

		case EWeaponType::E_Pistol:
			pistolAmmo = AmmoCalc(pistolAmmo);
			break;

		default:
			break;
		}
	}
}


void ASpeedShooterCharacter::ManualReloadWeapon()
{
	ReloadWeapon(Weapons[WeaponIndex]->weaponType);
}

int ASpeedShooterCharacter::AmmoCalc(int _ammoCount)
{
	if(_ammoCount > 0)
	{
		//Check if there is room in the clip to reload.
		if (Weapons[WeaponIndex]-> clipAmmo != Weapons[WeaponIndex]->maxClipAmmo)
		{
			// Check if there is enough ammo to reload the clip.
			if(_ammoCount - (Weapons[WeaponIndex]->maxClipAmmo - Weapons[WeaponIndex]->clipAmmo) >= 0)
			{
				_ammoCount -= (Weapons[WeaponIndex]->maxClipAmmo - Weapons[WeaponIndex]->clipAmmo);
				Weapons[WeaponIndex]->clipAmmo = Weapons[WeaponIndex]-> maxClipAmmo;
			}
			else
			{
				Weapons[WeaponIndex]->clipAmmo += _ammoCount;
				_ammoCount = 0;
			}
		}
	}
	else
	{
		// Informs the player there is no ammo in inventory or clip.
		OutOfAmmoPopUp();
	}

	return _ammoCount;
}

void ASpeedShooterCharacter::SwitchWeapon()
{
	switch (WeaponIndex)
	{
		case 0:
			if (Weapons.Num() > 1)
			{
				WeaponIndex = 1;
				SwitchWeaponMesh(WeaponIndex);
			}
			else
			{
				WeaponIndex = 0;
				SwitchWeaponMesh(WeaponIndex);
			}
			break;
		
		case 1:
			if (Weapons.Num() > 2)
			{
				WeaponIndex = 2;
				SwitchWeaponMesh(WeaponIndex);
			}
			else
			{
				WeaponIndex = 0;
				SwitchWeaponMesh(WeaponIndex);
			}
			break;
		
		case 2:
			if (Weapons.Num() > 3)
			{
				WeaponIndex = 3;
				SwitchWeaponMesh(WeaponIndex);
			}
			else
			{
				WeaponIndex = 0;
				SwitchWeaponMesh(WeaponIndex);
			}
			break;
		
		default:
			break;
		}
}

void ASpeedShooterCharacter::AddAmmo(EAmmoType _ammoType, int _ammoCount)
{
	switch (_ammoType)
	{
		case EAmmoType::E_Rifle:
			rifleAmmo += _ammoCount;
			break;

		case EAmmoType::E_Pistol:
			pistolAmmo += _ammoCount;
			break;

		default:
			break;
		
	}
}


