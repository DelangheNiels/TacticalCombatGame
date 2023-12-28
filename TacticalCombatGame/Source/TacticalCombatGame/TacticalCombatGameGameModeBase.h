// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TacticalCombatGameGameModeBase.generated.h"

class UStateMachineComponent;
class UPlayerTurnState;
class UEnemyTurnState;
class UEndTurnButton;
class UGameInformation;
class AEnemyPlayer;
class APlayerPawn;

DECLARE_MULTICAST_DELEGATE(FOnNewTurn);

UCLASS()
class TACTICALCOMBATGAME_API ATacticalCombatGameGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	ATacticalCombatGameGameModeBase();

public:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void ChangeTurn();
	void IncreaseTurnCount();
	int GetTurnCount() const;

	void CreateEnemyPlayerStates(AEnemyPlayer* enemyPlayer);
	void CreatePlayerStates(APlayerPawn* playerPawn);

	FOnNewTurn OnNewTurn;

private:

	bool _isItPlayerTurn;

	int _turnCount;

	bool _isPlayerFound;
	bool _isEnemyFound;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UEndTurnButton> _endTurnButtonRef;
	UPROPERTY()
		UEndTurnButton* _endTurnButton;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UGameInformation> _gameInformationUIRef;
	UPROPERTY()
		UGameInformation* _gameInformationUI;

	//States
	UPROPERTY()
		UStateMachineComponent* _stateMachine;
	UPROPERTY()
		UPlayerTurnState* _playerTurnState;
	UPROPERTY()
		UEnemyTurnState* _enemyTurnState;
	
	//Functions
	bool IsItPlayerTurn();
	bool IsItEnemyTurn();

	void CreateStates();
	void CreateTransitions();

	void CreateUI();
};
