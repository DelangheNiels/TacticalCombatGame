// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseAttack.generated.h"

class AGridTile;
class AGrid;

UCLASS()
class TACTICALCOMBATGAME_API UBaseAttack : public UObject
{
	GENERATED_BODY()

public:
	void ShowTilesToAttack(const FVector2D& currentTileIndex, const FVector2D& characterForwardVector);
	void HideTilesToAttack();
	void Attack(float damage);
	void SetGrid(AGrid* grid);

protected:

	UPROPERTY()
		TArray<AGridTile*> _tilesToAttack;

	UPROPERTY()
		AGrid* _grid;

	//Functions

	virtual TArray<AGridTile*> FindTilesToAttack(const FVector2D& currentTileIndex, const FVector2D& characterForwardVector) PURE_VIRTUAL(UBaseAttack::FindTilesToAttack, return TArray<AGridTile*>(););

};
