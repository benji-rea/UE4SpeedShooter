// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleAmmo.h"

// Sets default values
ASimpleAmmo::ASimpleAmmo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ammoType = EAmmoType::E_Rifle;
	ammoCount = 20;

}

// Called when the game starts or when spawned
void ASimpleAmmo::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASimpleAmmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

