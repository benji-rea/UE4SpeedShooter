// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SimpleEnemy.generated.h"

UCLASS()
class SPEEDSHOOTER_API ASimpleEnemy : public ACharacter
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASimpleEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void TakeDamage(float _damage);

	// Enemy Health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enemy)
	float Health;

	// Hit Detector
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = Enemy)
	bool hasDamage;

	// Dead Detector
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = Enemy)
	bool isDead;
};
