// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SpeedShooterGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SPEEDSHOOTER_API ASpeedShooterGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
	virtual void StartPlay() override;

};
