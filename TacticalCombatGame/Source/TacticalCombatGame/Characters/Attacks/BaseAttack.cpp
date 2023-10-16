// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAttack.h"

#include "../../Grid/Grid.h"
#include "../../Grid/GridTile.h"

void UBaseAttack::ShowTilesToAttack(const FVector2D& currentTileIndex, const FVector2D& characterForwardVector)
{
	_tilesToAttack.Empty();

	_tilesToAttack = FindTilesToAttack(currentTileIndex, characterForwardVector);

	//use grid to show tile visuals
	for (int i = 0; i < _tilesToAttack.Num(); i++)
	{
		_grid->SetTileReachable(_tilesToAttack[i]);
	}
}

void UBaseAttack::HideTilesToAttack()
{
	//use grid visuals to remove tile visuals
	_grid->ResetGridVisuals();

	_tilesToAttack.Empty();
}

void UBaseAttack::Attack(float damage)
{
	if (_tilesToAttack.Num() <= 0)
		return;

	//see if there is a char on the tile, if so deal damage to it
}

void UBaseAttack::SetGrid(AGrid* grid)
{
	_grid = grid;
}
