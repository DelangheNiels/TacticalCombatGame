// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "State.h"
#include "PlayerTurnState.generated.h"

class ATacticalCombatGameGameModeBase;
class APlayerPawn;

UCLASS()
class TACTICALCOMBATGAME_API UPlayerTurnState : public UState
{
	GENERATED_BODY()

public:

	UPlayerTurnState();

public:
	void Initialize(ATacticalCombatGameGameModeBase* gamemode, APlayerPawn* playerPawn);
	void OnEnter() override;
	void Update(float deltaTime) override;
	void OnExit() override;


private:
	
	UPROPERTY()
		ATacticalCombatGameGameModeBase* _gamemode;

	UPROPERTY()
		APlayerPawn* _playerPawn;

};
