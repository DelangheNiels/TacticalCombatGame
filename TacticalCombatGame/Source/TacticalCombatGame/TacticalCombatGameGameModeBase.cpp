// Copyright Epic Games, Inc. All Rights Reserved.


#include "TacticalCombatGameGameModeBase.h"

#include "StateMachine/StateMachineComponent.h"
#include "StateMachine/PlayerTurnState.h"
#include "StateMachine/EnemyTurnState.h"

#include "UI/EndTurnButton.h"
#include "UI/GameInformation.h"

#include "Kismet/GameplayStatics.h"

#include "Enemy/EnemyPlayer.h"

ATacticalCombatGameGameModeBase::ATacticalCombatGameGameModeBase()
{
	_stateMachine = CreateDefaultSubobject<UStateMachineComponent>(TEXT("StateMachineComponent"));

	_isItPlayerTurn = true;
	_turnCount = 1;
}

void ATacticalCombatGameGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (!_stateMachine)
		return;

	CreateUI();
}

void ATacticalCombatGameGameModeBase::Tick(float DeltaTime)
{
	if (_stateMachine)
		return;
	_stateMachine->UpdateCurrentState(DeltaTime);
}

void ATacticalCombatGameGameModeBase::ChangeTurn()
{
	_isItPlayerTurn = !_isItPlayerTurn;
}

void ATacticalCombatGameGameModeBase::IncreaseTurnCount()
{
	++_turnCount;
	OnNewTurn.Broadcast();
}

int ATacticalCombatGameGameModeBase::GetTurnCount() const
{
	return _turnCount;
}

void ATacticalCombatGameGameModeBase::CreateEnemyPlayerStates(AEnemyPlayer* enemyPlayer)
{
	_isEnemyFound = true;

	_enemyTurnState = NewObject<UEnemyTurnState>();
	_enemyTurnState->Initialize(enemyPlayer);

	if(_isPlayerFound && _isEnemyFound)
		CreateTransitions();

}

void ATacticalCombatGameGameModeBase::CreatePlayerStates(APlayerPawn* playerPawn)
{
	_isPlayerFound = true;

	_playerTurnState = NewObject<UPlayerTurnState>();
	_playerTurnState->Initialize(this, playerPawn);

	_stateMachine->SetInitialState(_playerTurnState);

	if (_isPlayerFound && _isEnemyFound)
		CreateTransitions();
}

bool ATacticalCombatGameGameModeBase::IsItPlayerTurn()
{
	if (_isItPlayerTurn)
		return true;

	return false;
}

bool ATacticalCombatGameGameModeBase::IsItEnemyTurn()
{
	if (_isItPlayerTurn)
		return false;

	return true;
}

void ATacticalCombatGameGameModeBase::CreateTransitions()
{
	TFunction<bool()> changeToEnemyTurn = [this]() {return IsItEnemyTurn(); };
	TFunction<bool()> changeToPlayerTurn = [this]() {return IsItPlayerTurn(); };

	_stateMachine->AddStateTransition(_playerTurnState, _enemyTurnState, changeToEnemyTurn);
	_stateMachine->AddStateTransition(_enemyTurnState, _playerTurnState, changeToPlayerTurn);
}

void ATacticalCombatGameGameModeBase::CreateUI()
{
	if (!_endTurnButtonRef)
		return;
	_endTurnButton = Cast<UEndTurnButton>(CreateWidget(GetWorld(), _endTurnButtonRef));
	_endTurnButton->AddToViewport();

	if (!_gameInformationUIRef)
		return;
	_gameInformationUI = Cast<UGameInformation>(CreateWidget(GetWorld(), _gameInformationUIRef));
	_gameInformationUI->AddToViewport();
}
