// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameInformation.generated.h"

class UTextBlock;
class ATacticalCombatGameGameModeBase;

UCLASS()
class TACTICALCOMBATGAME_API UGameInformation : public UUserWidget
{
	GENERATED_BODY()

public:

	void NativeConstruct() override;
	void NativeDestruct() override;

private:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
		UTextBlock* _turnAmountText;

	UPROPERTY()
		ATacticalCombatGameGameModeBase* _gamemode;

	//Functions

	void OnTurnChanged();
	
};
