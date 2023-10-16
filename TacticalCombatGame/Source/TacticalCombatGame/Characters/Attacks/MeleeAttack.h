// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAttack.h"
#include "MeleeAttack.generated.h"

class AGridTile;

UCLASS(Blueprintable)
class TACTICALCOMBATGAME_API UMeleeAttack : public UBaseAttack
{
	GENERATED_BODY()

protected:

	virtual TArray<AGridTile*> FindTilesToAttack(const FVector2D& currentTileIndex, const FVector2D& characterForwardVector) override;
	
};
