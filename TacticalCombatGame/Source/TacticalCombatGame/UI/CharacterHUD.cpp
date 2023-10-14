// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterHUD.h"

#include "../Characters/BaseCharacter.h"

#include "Components/Button.h"

void UCharacterHUD::NativeConstruct()
{
	Super::NativeConstruct();

	_moveButton->OnClicked.AddDynamic(this, &UCharacterHUD::OnMove);
	_attackButton->OnClicked.AddDynamic(this, &UCharacterHUD::OnAttack);
}

void UCharacterHUD::SetCharacter(ABaseCharacter* character)
{
	_character = character;
}

void UCharacterHUD::OnMove()
{
	if (!_character)
		return;

	_character->ShowReachableTiles();
}

void UCharacterHUD::OnAttack()
{
	if (!_character)
		return;

	_character->HideReachableTiles();

	//show tile to deal damage to
	//show confirm button
}
