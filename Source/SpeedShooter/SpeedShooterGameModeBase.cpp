// Copyright Epic Games, Inc. All Rights Reserved.


#include "SpeedShooterGameModeBase.h"

void ASpeedShooterGameModeBase::StartPlay()
{
	Super::StartPlay();

	check(GEngine != nullptr);

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Hello World, this is a FPS Game!"));

}