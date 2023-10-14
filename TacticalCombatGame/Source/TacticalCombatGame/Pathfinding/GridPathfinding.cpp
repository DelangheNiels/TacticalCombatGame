// Fill out your copyright notice in the Description page of Project Settings.


#include "GridPathfinding.h"

#include "../Grid/Grid.h"
#include "../Grid/GridTile.h"

#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values
AGridPathfinding::AGridPathfinding()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGridPathfinding::BeginPlay()
{
	Super::BeginPlay();
	
	_grid = Cast<AGrid>(UGameplayStatics::GetActorOfClass(GetWorld(), AGrid::StaticClass()));

	if (!_grid)
		return;

	SetNeighborsForTiles();
}

// Called every frame
void AGridPathfinding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TArray<AGridTile*> AGridPathfinding::GeneratePath(AGridTile* startTile, AGridTile* destinationTile)
{
	TArray<AGridTile*> path;
	TArray<AGridTile*> checkedTiles;

	bool isPathFound = false;
	
	//empty path when a tile is invalid or start and destination are the same
	if (startTile == nullptr || destinationTile == nullptr || startTile == destinationTile)
		return path;

	path.Add(startTile);

	AGridTile* tileToCheck;

	while (!isPathFound)
	{
		//Get next tile to check
		tileToCheck = GetTileToCheck(path, startTile);
		
		//look at neighbors of tile
		//calc dist for each of these tiles to destination
		//take tile with smallest distance
		AGridTile* closestTile = GetClosestNeighborToDestination(*tileToCheck, *destinationTile, checkedTiles);
		checkedTiles.Add(tileToCheck);
		path.Add(closestTile);

		//repeat until tile equals destination
		if (closestTile == destinationTile)
		{
			isPathFound = true;
			break;
		}
	}

	checkedTiles.Empty();

	return path;

}

void AGridPathfinding::SetNeighborsForTiles()
{
	TMap<FVector2D, AGridTile*> tileMap = _grid->GetTileIndexMap();

	for (auto& element : tileMap)
	{
		auto tile = element.Value;
		const FVector2D index = tile->GetGridIndex();
		//top :  x +1
		tile->AddNeighbor(FindTileByIndex(index + FVector2D(1, 0)));
		//bottom : x -1
		tile->AddNeighbor(FindTileByIndex(index + FVector2D(-1, 0)));
		//left : y - 1
		tile->AddNeighbor(FindTileByIndex(index + FVector2D(0, -1)));
		//right : y + 1
		tile->AddNeighbor(FindTileByIndex(index + FVector2D(0, 1)));

	}
}

AGridTile* AGridPathfinding::FindTileByIndex(const FVector2D& index)
{
	TMap<FVector2D, AGridTile*> tileMap = _grid->GetTileIndexMap();

	auto* tile = tileMap.Find(index);
	if (tile == nullptr)
		return nullptr;

	return *tile;
}

AGridTile* AGridPathfinding::GetTileToCheck(const TArray<AGridTile*>& path, AGridTile* startTile)
{
	if (path.Num() <= 1)
		return startTile;
	else
		return path[path.Num() - 1];
}

AGridTile* AGridPathfinding::GetClosestNeighborToDestination(const AGridTile& tile, const AGridTile& destination, TArray<AGridTile*>& checkedTiles)
{
	AGridTile* closestTile = nullptr;
	int closestDistance = MAX_int32;

	for (int i = 0; i < tile.GetNeighbors().Num(); i++)
	{
		AGridTile* tileToCheck = tile.GetNeighbors()[i];

		if (checkedTiles.Contains(tileToCheck))
			continue;

		checkedTiles.Add(tileToCheck);

		int distance = CalculateDistanceToTarget(*tileToCheck, destination);

		if (distance >= closestDistance)
			continue;

		closestDistance = distance;
		closestTile = tileToCheck;
	}

	return closestTile;
}

int AGridPathfinding::CalculateDistanceToTarget(const AGridTile& tile, const AGridTile& target)
{
	//distance is getting calculated by getting the difference between the row and column positions of the 2 tiles
	int distX = FMath::Abs(target.GetGridIndex().X - tile.GetGridIndex().X);
	int distY = FMath::Abs(target.GetGridIndex().Y - tile.GetGridIndex().Y);
	return distX + distY;
}



