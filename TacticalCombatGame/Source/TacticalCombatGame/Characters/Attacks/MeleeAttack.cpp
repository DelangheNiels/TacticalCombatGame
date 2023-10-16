// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeAttack.h"

#include "../../Grid/Grid.h"
#include "../../Grid/GridTile.h"

TArray<AGridTile*> UMeleeAttack::FindTilesToAttack(const FVector2D& currentTileIndex, const FVector2D& characterForwardVector)
{
	TArray<AGridTile*> tilesToAttack;

	FVector2D targetTileIndex = currentTileIndex + characterForwardVector;

	AGridTile* tile = _grid->FindTileByIndex(targetTileIndex);

	if (tile)
		tilesToAttack.Add(tile);

	return tilesToAttack;
}

