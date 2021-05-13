// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SimpleAmmo.generated.h"

UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	E_Rifle		UMETA(DisplayName = "RIFLE"),
	E_Pistol	UMETA(DisplayName = "PISTOL")
};

UCLASS()
class SPEEDSHOOTER_API ASimpleAmmo : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASimpleAmmo();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Total Ammo in Pickup
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int ammoCount;

	// Ammo type in Pickup
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EAmmoType ammoType;
};
