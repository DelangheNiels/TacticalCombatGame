// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyTurnState.h"

#include "../Enemy/EnemyPlayer.h"

#include "Kismet/GameplayStatics.h"

UEnemyTurnState::UEnemyTurnState()
{
}

void UEnemyTurnState::Initialize(AEnemyPlayer* enemyPlayer)
{
	_enemyPlayer = enemyPlayer;
}

void UEnemyTurnState::OnEnter()
{
	if (!_enemyPlayer)
		return;

	_enemyPlayer->ControlCharacters();
}

void UEnemyTurnState::Update(float deltaTime)
{
}

void UEnemyTurnState::OnExit()
{
	if (!_enemyPlayer)
		return;

	_enemyPlayer->ResetControlledCharacters();
}
