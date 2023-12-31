// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterHUD.generated.h"

class UButton;
class ABaseCharacter;
class UTextBlock;
class UProgressBar;
class UAttackComponent;
class UGridMovementComponent;
class UHealthComponent;

UCLASS()
class TACTICALCOMBATGAME_API UCharacterHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	void SetCharacter(ABaseCharacter* character);

private:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
		UButton* _moveButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
		UButton* _cancelMoveButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
		UButton* _confirmMoveButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
		UButton* _attackButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
		UButton* _cancelAttackButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
		UButton* _confirmAttackButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
		UTextBlock* _healthText;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
		UProgressBar* _healthBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
		UTextBlock* _movementText;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
		UProgressBar* _movementBar;

	UPROPERTY()
		ABaseCharacter* _character;

	UPROPERTY()
		UAttackComponent* _charAttackComp;

	bool _isLocked;

	//Functions
	UFUNCTION()
		void OnMove();
	UFUNCTION()
		void OnCancelMovement();
	UFUNCTION()
		void OnConfirmMovement();

	UFUNCTION()
		void OnAttack();
	UFUNCTION()
		void OnCancelAttack();
	UFUNCTION()
		void OnConfirmAttack();

	void SetButtonActiveInactive(UButton* button, bool active);

	void ShowDefaultUILayout();

	UFUNCTION()
		void SetCharacterHealth();
	UFUNCTION()
		void SetCharacterMovement();
	UFUNCTION()
		void OnCharacterRotated();

	UFUNCTION()
		void DisableInput();
	UFUNCTION()
		void EnableInput(UGridMovementComponent* movementComp = nullptr);

	UFUNCTION()
		void OnCharacterReset();
	UFUNCTION()
		void OnCharacterLocked();

	UFUNCTION()
		void OnChacterDied(UHealthComponent* healthcomp);

	
};
