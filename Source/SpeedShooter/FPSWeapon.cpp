// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSWeapon.h"

// Sets default values
AFPSWeapon::AFPSWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Variable Constructor
	maxTotalAmmo = 100;
	maxClipAmmo = 12;
	totalAmmo = 64;
	clipAmmo = 12;
	reloadTime = 1.0f;

	weaponType = EWeaponType::E_Rifle;

}

// Called when the game starts or when spawned
void AFPSWeapon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFPSWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

