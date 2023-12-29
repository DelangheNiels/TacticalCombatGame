// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAttack.h"

#include "../BaseCharacter.h"
#include "../HealthComponent.h"

#include "../../Grid/Grid.h"
#include "../../Grid/GridTile.h"

void UBaseAttack::ShowTilesToAttack(const FVector2D& currentTileIndex, const FVector2D& characterForwardVector)
{
	_tilesToAttack.Empty();

	_tilesToAttack = FindTilesToAttack(currentTileIndex, characterForwardVector);

	for (int i = 0; i < _tilesToAttack.Num(); i++)
	{
		_grid->SetTileReachable(_tilesToAttack[i]);
	}
}

void UBaseAttack::HideTilesToAttack()
{
	_grid->ResetGridVisuals();

	_tilesToAttack.Empty();
}

void UBaseAttack::Attack(float damage)
{
	if (_tilesToAttack.Num() <= 0)
		return;

	//see if there is a char on the tile, if so deal damage to it
	for (int i = 0; i < _tilesToAttack.Num(); i++)
	{
		auto tile = _tilesToAttack[i];

		if (tile == nullptr)
			continue;

		ABaseCharacter* hitCharacter = TryGettingCharacterOnTile(*tile);
		if (!hitCharacter)
			continue;

		hitCharacter->GetHealthComponent()->TakeDamage(damage);
	}
	
}

void UBaseAttack::SetGrid(AGrid* grid)
{
	_grid = grid;
}

int UBaseAttack::GetAttackRange() const
{
	return _attackRange;
}

int UBaseAttack::GetAttackOffset() const
{
	return _attackOffset;
}

TArray<AGridTile*> UBaseAttack::GetAllPossibleTilesToAttack(const FVector2D& currentTileIndex)
{
	TArray<AGridTile*> possibleTilesToAttack;
	possibleTilesToAttack += FindTilesToAttack(currentTileIndex, FVector2D(1, 0));
	possibleTilesToAttack += FindTilesToAttack(currentTileIndex, FVector2D(-1, 0));
	possibleTilesToAttack += FindTilesToAttack(currentTileIndex, FVector2D(0, 1));
	possibleTilesToAttack += FindTilesToAttack(currentTileIndex, FVector2D(0, -1));

	return possibleTilesToAttack;
}

ABaseCharacter* UBaseAttack::TryGettingCharacterOnTile(const AGridTile& tile)
{
	FVector position = tile.GetActorLocation();

	FHitResult hit;

	FVector traceStart = position + FVector(0, 0, 1000);
	FVector traceEnd = position + FVector(0, 0, -1000);
	FCollisionQueryParams queryParams;

	GetWorld()->LineTraceSingleByChannel(hit, traceStart, traceEnd, _characterTraceChannel, queryParams);

	if (!hit.bBlockingHit)
		return nullptr;

	return Cast<ABaseCharacter>(hit.GetActor());
}
