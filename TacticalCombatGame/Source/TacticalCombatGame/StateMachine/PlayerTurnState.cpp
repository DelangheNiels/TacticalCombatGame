// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerTurnState.h"
#include "State.h"

#include "../TacticalCombatGameGameModeBase.h"

#include "../Player/PlayerPawn.h"

UPlayerTurnState::UPlayerTurnState()
{
}

void UPlayerTurnState::Initialize(ATacticalCombatGameGameModeBase* gamemode, APlayerPawn* playerPawn)
{
	_gamemode = gamemode;
	_playerPawn = playerPawn;
}

void UPlayerTurnState::OnEnter()
{
	_gamemode->IncreaseTurnCount();

	_playerPawn->ResetControlledCharacters();
}

void UPlayerTurnState::Update(float deltaTime)
{
}

void UPlayerTurnState::OnExit()
{
	_playerPawn->LockControlledCharacters();
}
