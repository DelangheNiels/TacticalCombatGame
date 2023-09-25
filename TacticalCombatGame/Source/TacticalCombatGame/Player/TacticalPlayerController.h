// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TacticalPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALCOMBATGAME_API ATacticalPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	ATacticalPlayerController();

protected:

	virtual void BeginPlay() override;

public:

	virtual void SetupInputComponent() override;

private:
};
