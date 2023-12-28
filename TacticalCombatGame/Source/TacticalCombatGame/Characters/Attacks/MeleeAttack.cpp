// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeAttack.h"

#include "../../Grid/Grid.h"
#include "../../Grid/GridTile.h"

TArray<AGridTile*> UMeleeAttack::FindTilesToAttack(const FVector2D& currentTileIndex, const FVector2D& characterForwardVector)
{
	TArray<AGridTile*> tilesToAttack;

	FVector2D targetTileIndex = currentTileIndex + characterForwardVector;

	//round index to avoid wrong floating point numbers
	FVector2D inTargetTileIndex = FVector2D(FMath::RoundHalfFromZero(targetTileIndex.X), FMath::RoundHalfFromZero(targetTileIndex.Y));

	AGridTile* tile = _grid->FindTileByIndex(inTargetTileIndex);

	if (tile)
		tilesToAttack.Add(tile);

	return tilesToAttack;
}

