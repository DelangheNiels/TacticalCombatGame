// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterHUD.h"

#include "../Characters/BaseCharacter.h"
#include "../Characters/HealthComponent.h"

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

	SetButtonActiveInactive(_cancelMoveButton, false);
	SetButtonActiveInactive(_confirmMoveButton, false);
}

void UCharacterHUD::NativeDestruct()
{
	Super::NativeDestruct();

	_character->OnCharacterMoved.RemoveAll(this);
	_character->GetHealthComponent()->OnHealthChanged.RemoveAll(this);

	_character = nullptr;
}

void UCharacterHUD::SetCharacter(ABaseCharacter* character)
{
	_character = character;

	_character->OnCharacterMoved.AddUObject(this, &UCharacterHUD::SetCharacterMovement);
	_character->GetHealthComponent()->OnHealthChanged.AddUObject(this, &UCharacterHUD::SetCharacterHealth);

	SetCharacterHealth();
	SetCharacterMovement();
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

void UCharacterHUD::SetCharacterHealth()
{
	auto healthComp = _character->GetHealthComponent();
	FText text = FText::FromString(FString::Printf(TEXT("%i/%i"), int(healthComp->GetCurrentHealth()), int(healthComp->GetMaxHealth())));
	_healthText->SetText(text);

	_healthBar->SetPercent(healthComp->GetCurrentHealth() / healthComp->GetMaxHealth());
}

void UCharacterHUD::SetCharacterMovement()
{
	FText text = FText::FromString(FString::Printf(TEXT("%i/%i"), _character->GetCurrentAmountOfMovement(), _character->GetTotalAmountOfMovement()));
	_movementText->SetText(text);

	_movementBar->SetPercent(float(_character->GetCurrentAmountOfMovement()) / float(_character->GetTotalAmountOfMovement()));
}
