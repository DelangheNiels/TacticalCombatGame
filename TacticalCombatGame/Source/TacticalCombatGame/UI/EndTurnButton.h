// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndTurnButton.generated.h"

class UButton;
class ATacticalCombatGameGameModeBase;

UCLASS()
class TACTICALCOMBATGAME_API UEndTurnButton : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
		UButton* _endTurnButton;

	UPROPERTY()
		ATacticalCombatGameGameModeBase* _gamemode;

	//functions
	UFUNCTION()
		void EndTurn();

	UFUNCTION()
		void EnableButton();
	
};
