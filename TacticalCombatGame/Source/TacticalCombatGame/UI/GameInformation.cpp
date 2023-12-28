// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInformation.h"

#include "Kismet/GameplayStatics.h"

#include "../TacticalCombatGameGameModeBase.h"

#include "Components/TextBlock.h"

void UGameInformation::NativeConstruct()
{
	_gamemode = Cast<ATacticalCombatGameGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	if (!_gamemode)
		return;

	_gamemode->OnNewTurn.AddUObject(this, &UGameInformation::OnTurnChanged);

	OnTurnChanged();
}

void UGameInformation::NativeDestruct()
{
	if (!_gamemode)
		return;

	_gamemode->OnNewTurn.RemoveAll(this);
}

void UGameInformation::OnTurnChanged()
{
	if (!_gamemode)
		return;
	FText turnText = FText::FromString(FString::Printf(TEXT("Turn: %i"),_gamemode->GetTurnCount()));
	_turnAmountText->SetText(turnText);
}
