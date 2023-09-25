// Fill out your copyright notice in the Description page of Project Settings.


#include "TacticalPlayerController.h"

ATacticalPlayerController::ATacticalPlayerController()
{

}

void ATacticalPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//Show mouse cursor when game is running
	bShowMouseCursor = true;
}
