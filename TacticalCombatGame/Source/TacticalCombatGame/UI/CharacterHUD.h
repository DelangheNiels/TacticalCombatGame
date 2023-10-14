// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterHUD.generated.h"

class UButton;
class ABaseCharacter;

UCLASS()
class TACTICALCOMBATGAME_API UCharacterHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	
	virtual void NativeConstruct() override;

	void SetCharacter(ABaseCharacter* character);

private:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
		UButton* _moveButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
		UButton* _attackButton;

	UPROPERTY()
		ABaseCharacter* _character;

	//Functions
	UFUNCTION()
		void OnMove();

	UFUNCTION()
		void OnAttack();

	
};
