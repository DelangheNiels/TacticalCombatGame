// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterHUD.h"

#include "../Characters/BaseCharacter.h"

#include "Components/Button.h"

void UCharacterHUD::NativeConstruct()
{
	Super::NativeConstruct();

	_moveButton->OnClicked.AddDynamic(this, &UCharacterHUD::OnMove);
	_confirmMoveButton->OnClicked.AddDynamic(this, &UCharacterHUD::OnConfirmMovement);
	_cancelMoveButton->OnClicked.AddDynamic(this, &UCharacterHUD::OnCancelMovement);

	_attackButton->OnClicked.AddDynamic(this, &UCharacterHUD::OnAttack);

	SetButtonActiveInactive(_cancelMoveButton, false);
	SetButtonActiveInactive(_confirmMoveButton, false);
}

void UCharacterHUD::SetCharacter(ABaseCharacter* character)
{
	_character = character;
}

void UCharacterHUD::OnMove()
{
	if (!_character)
		return;

	SetButtonActiveInactive(_moveButton, false);
	SetButtonActiveInactive(_attackButton, false);
	SetButtonActiveInactive(_cancelMoveButton, true);
	SetButtonActiveInactive(_confirmMoveButton, true);
	
	_character->ShowReachableTiles();
}

void UCharacterHUD::OnCancelMovement()
{
	ShowDefaultUILayout();

}

void UCharacterHUD::OnConfirmMovement()
{
	ShowDefaultUILayout();

	_character->MoveToDestination();
}

void UCharacterHUD::OnAttack()
{
	if (!_character)
		return;

	_character->HideReachableTiles();

	//show tile to deal damage to
	//show confirm button
}

void UCharacterHUD::SetButtonActiveInactive(UButton* button, bool active)
{
	button->SetIsEnabled(active);
	if(active)
		button->SetVisibility(ESlateVisibility::Visible);
	else
		button->SetVisibility(ESlateVisibility::Hidden);
}

void UCharacterHUD::ShowDefaultUILayout()
{
	_character->ClearVisuals();

	SetButtonActiveInactive(_moveButton, true);
	SetButtonActiveInactive(_attackButton, true);
	SetButtonActiveInactive(_cancelMoveButton, false);
	SetButtonActiveInactive(_confirmMoveButton, false);
}
