// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterHUD.h"

#include "../Characters/BaseCharacter.h"
#include "../Characters/HealthComponent.h"
#include "../Characters/GridMovementComponent.h"
#include "../Characters/Attacks/AttackComponent.h"

#include "../Grid/GridTile.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UCharacterHUD::NativeConstruct()
{
	Super::NativeConstruct();

	_moveButton->OnClicked.AddDynamic(this, &UCharacterHUD::OnMove);
	_confirmMoveButton->OnClicked.AddDynamic(this, &UCharacterHUD::OnConfirmMovement);
	_cancelMoveButton->OnClicked.AddDynamic(this, &UCharacterHUD::OnCancelMovement);

	_attackButton->OnClicked.AddDynamic(this, &UCharacterHUD::OnAttack);
	_cancelAttackButton->OnClicked.AddDynamic(this, &UCharacterHUD::OnCancelAttack);
	_confirmAttackButton->OnClicked.AddDynamic(this, &UCharacterHUD::OnConfirmAttack);

	ShowDefaultUILayout();
}

void UCharacterHUD::NativeDestruct()
{
	Super::NativeDestruct();

	_character->OnCharacterLocked.RemoveAll(this);
	_character->OnCharacterReset.RemoveAll(this);
	_character->GetGridMovementComponent()->OnCharacterMoved.RemoveAll(this);
	_character->GetGridMovementComponent()->OnCharacterStopedMoving.RemoveAll(this);
	_character->GetHealthComponent()->OnHealthChanged.RemoveAll(this);
	_character->OnCharacterRotated.RemoveAll(this);

	_character = nullptr;
	_charAttackComp = nullptr;
}

void UCharacterHUD::SetCharacter(ABaseCharacter* character)
{
	_character = character;

	_charAttackComp = character->GetAttackComponent();
	
	_character->OnCharacterLocked.AddUObject(this, &UCharacterHUD::OnCharacterLocked);
	_character->OnCharacterReset.AddUObject(this, &UCharacterHUD::OnCharacterReset);
	_character->GetGridMovementComponent()->OnCharacterMoved.AddUObject(this, &UCharacterHUD::SetCharacterMovement);
	_character->GetGridMovementComponent()->OnCharacterMoved.AddUObject(this, &UCharacterHUD::DisableInput);
	_character->GetGridMovementComponent()->OnCharacterStopedMoving.AddUObject(this, &UCharacterHUD::EnableInput);
	_character->GetHealthComponent()->OnHealthChanged.AddUObject(this, &UCharacterHUD::SetCharacterHealth);
	_character->OnCharacterRotated.AddUObject(this, &UCharacterHUD::OnCharacterRotated);

	SetCharacterHealth();
	SetCharacterMovement();

	ShowDefaultUILayout();
}

void UCharacterHUD::OnMove()
{
	if (!_character)
		return;

	SetButtonActiveInactive(_moveButton, false);
	SetButtonActiveInactive(_attackButton, false);
	SetButtonActiveInactive(_cancelMoveButton, true);
	SetButtonActiveInactive(_confirmMoveButton, true);
	
	_character->GetGridMovementComponent()->ShowReachableTiles();
}

void UCharacterHUD::OnCancelMovement()
{
	ShowDefaultUILayout();

}

void UCharacterHUD::OnConfirmMovement()
{
	ShowDefaultUILayout();

	if (!_character)
		return;

	_character->GetGridMovementComponent()->MoveToDestination();
}

void UCharacterHUD::OnAttack()
{
	if (!_character || !_charAttackComp)
		return;

	_character->GetGridMovementComponent()->HideReachableTiles();

	_charAttackComp->ShowTilesToAttack(_character->GetGridMovementComponent()->GetCurrentTile()->GetGridIndex(), (FVector2D)_character->GetActorForwardVector());

	SetButtonActiveInactive(_moveButton, false);
	SetButtonActiveInactive(_attackButton, false);
	SetButtonActiveInactive(_cancelAttackButton, true);
	SetButtonActiveInactive(_confirmAttackButton, true);

}

void UCharacterHUD::OnCancelAttack()
{
	if (!_charAttackComp)
		return;

	_charAttackComp->HideTilesToAttack();

	ShowDefaultUILayout();
}

void UCharacterHUD::OnConfirmAttack()
{
	if (!_charAttackComp)
		return;

	_charAttackComp->DoAttack();
	_charAttackComp->HideTilesToAttack();
	
	ShowDefaultUILayout();

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
	SetButtonActiveInactive(_moveButton, true);
	SetButtonActiveInactive(_attackButton, true);
	SetButtonActiveInactive(_cancelMoveButton, false);
	SetButtonActiveInactive(_confirmMoveButton, false);
	SetButtonActiveInactive(_cancelAttackButton, false);
	SetButtonActiveInactive(_confirmAttackButton, false);

	if (_isLocked || !_character->GetIsControlledByPlayer())
	{
		DisableInput();
		return;
	}

	if (_charAttackComp->GetHasAttacked())
		_attackButton->SetIsEnabled(false);

	if (_character->GetGridMovementComponent()->GetCurrentAmountOfMovement() == 0)
		_moveButton->SetIsEnabled(false);

	if (!_character)
		return;

	_character->GetGridMovementComponent()->ClearVisuals();
}

void UCharacterHUD::SetCharacterHealth()
{
	if (!_character)
		return;

	auto healthComp = _character->GetHealthComponent();
	FText text = FText::FromString(FString::Printf(TEXT("%i/%i"), int(healthComp->GetCurrentHealth()), int(healthComp->GetMaxHealth())));
	_healthText->SetText(text);

	_healthBar->SetPercent(healthComp->GetCurrentHealth() / healthComp->GetMaxHealth());
}

void UCharacterHUD::SetCharacterMovement()
{
	FText text = FText::FromString(FString::Printf(TEXT("%i/%i"), _character->GetGridMovementComponent()->GetCurrentAmountOfMovement(), _character->GetGridMovementComponent()->GetTotalAmountOfMovement()));
	_movementText->SetText(text);

	_movementBar->SetPercent(float(_character->GetGridMovementComponent()->GetCurrentAmountOfMovement()) / float(_character->GetGridMovementComponent()->GetTotalAmountOfMovement()));


}

void UCharacterHUD::OnCharacterRotated()
{
	if (!_character || !_charAttackComp || !_charAttackComp->GetWantsToAttack())
		return;

	//hide previous tile
	_charAttackComp->HideTilesToAttack();

	//show new tiles to attack
	_charAttackComp->ShowTilesToAttack(_character->GetGridMovementComponent()->GetCurrentTile()->GetGridIndex(), (FVector2D)_character->GetActorForwardVector());
}

void UCharacterHUD::DisableInput()
{
	_moveButton->SetIsEnabled(false);
	_attackButton->SetIsEnabled(false);
}

void UCharacterHUD::EnableInput()
{
	_moveButton->SetIsEnabled(true);
	_attackButton->SetIsEnabled(true);

	if (_charAttackComp->GetHasAttacked())
		_attackButton->SetIsEnabled(false);

	if (_character->GetGridMovementComponent()->GetCurrentAmountOfMovement() == 0)
		_moveButton->SetIsEnabled(false);
}

void UCharacterHUD::OnCharacterReset()
{
	EnableInput();
	_isLocked = false;
}

void UCharacterHUD::OnCharacterLocked()
{
	DisableInput();
	_isLocked = true;
}

