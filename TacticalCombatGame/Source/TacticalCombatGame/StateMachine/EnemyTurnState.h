// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "State.h"
#include "EnemyTurnState.generated.h"

class AEnemyPlayer;

UCLASS()
class TACTICALCOMBATGAME_API UEnemyTurnState : public UState
{
	GENERATED_BODY()

public:

	UEnemyTurnState();

public:

	void Initialize(AEnemyPlayer* enemyPlayer);
	void OnEnter() override;
	void Update(float deltaTime) override;
	void OnExit() override;

private:
	
	UPROPERTY()
		AEnemyPlayer* _enemyPlayer;

};
