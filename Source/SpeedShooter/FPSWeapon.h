// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSWeapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
		E_Rifle		UMETA(DisplayName = "RIFLE"),
        E_Pistol	UMETA(DisplayName = "PISTOL")
};


UCLASS()
class SPEEDSHOOTER_API AFPSWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSWeapon();

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = Weapon)
	int maxTotalAmmo;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = Weapon)
	int maxClipAmmo;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = Weapon)
	int totalAmmo;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = Weapon)
	int clipAmmo;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = Weapon)
	float reloadTime;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = Weapon)
	EWeaponType weaponType;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
