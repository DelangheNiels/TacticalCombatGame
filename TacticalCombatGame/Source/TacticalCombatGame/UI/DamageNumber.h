// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageNumber.generated.h"

class UTextBlock;

UCLASS()
class TACTICALCOMBATGAME_API UDamageNumber : public UUserWidget
{
	GENERATED_BODY()

public:

	void Instantiate(float damage, const FVector& location);

private:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
		UTextBlock* _damageText;

	void SetDamageNumberText(float damage);
	void SetCorrectLocation(const FVector& location);
	
};
