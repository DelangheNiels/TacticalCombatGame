// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "State.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class TACTICALCOMBATGAME_API UState : public UObject
{
	GENERATED_BODY()

public:

	UState();

public:

	UFUNCTION()
		virtual void OnEnter() PURE_VIRTUAL(UState::OnEnter,);
	UFUNCTION()
		virtual void Update(float deltaTime)PURE_VIRTUAL(UState::Update, );
	UFUNCTION()
		virtual void OnExit()PURE_VIRTUAL(UState::OnExit, );
};
