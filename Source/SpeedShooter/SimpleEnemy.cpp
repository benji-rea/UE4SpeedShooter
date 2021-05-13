// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleEnemy.h"

// Sets default values
ASimpleEnemy::ASimpleEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Constructor Health
	Health = 1.0f;
	hasDamage = false;
	isDead = false;

}

// Called when the game starts or when spawned
void ASimpleEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASimpleEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASimpleEnemy::TakeDamage(float _damage)
{
	Health -= _damage;

	if(Health < 0.0f)
	{
		isDead = true;
	}
	else
	{
		hasDamage = true;	
	}
}

