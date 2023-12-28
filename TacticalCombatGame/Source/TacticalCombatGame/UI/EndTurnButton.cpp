// Fill out your copyright notice in the Description page of Project Settings.


#include "EndTurnButton.h"

#include "Components/Button.h"

#include "Kismet/GameplayStatics.h"

#include "../TacticalCombatGameGameModeBase.h"

void UEndTurnButton::NativeConstruct()
{
	Super::NativeConstruct();


	_gamemode = Cast<ATacticalCombatGameGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	if (!_gamemode)
		return;

	_endTurnButton->OnClicked.AddDynamic(this, &UEndTurnButton::EndTurn);

	_gamemode->OnNewTurn.AddUObject(this, &UEndTurnButton::EnableButton);

}

void UEndTurnButton::NativeDestruct()
{
	if (!_gamemode)
		return;

	_gamemode->OnNewTurn.RemoveAll(this);
}

void UEndTurnButton::EndTurn()
{
	if (!_gamemode)
		return;

	_gamemode->ChangeTurn();

	_endTurnButton->SetIsEnabled(false);
}

void UEndTurnButton::EnableButton()
{
	_endTurnButton->SetIsEnabled(true);
}
