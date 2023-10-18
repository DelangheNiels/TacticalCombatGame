// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageNumber.h"

#include "Components/TextBlock.h"

#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

void UDamageNumber::Instantiate(float damage, const FVector& location)
{
	SetDamageNumberText(damage);
	SetCorrectLocation(location);
}

void UDamageNumber::SetDamageNumberText(float damage)
{
	FText text = FText::FromString(FString::Printf(TEXT("%i"), int(damage)));
	_damageText->SetText(text);
}

void UDamageNumber::SetCorrectLocation(const FVector& location)
{
	auto playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FVector2D screenLocation;

	playerController->ProjectWorldLocationToScreen(location, screenLocation, true);

	SetPositionInViewport(screenLocation);
}
