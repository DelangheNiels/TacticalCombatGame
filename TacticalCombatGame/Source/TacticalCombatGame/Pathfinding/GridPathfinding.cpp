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
		tileToCheck = GetTileToCheck(path, startTile);
		
		AGridTile* closestTile = GetClosestNeighborToDestination(*tileToCheck, *destinationTile, checkedTiles);
		if (closestTile == nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT("Error creating path"));
			return path;
		}
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

	ShowPath(path);

	return path;

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
	if (&tile == nullptr || &destination == nullptr)
		return nullptr;

	AGridTile* closestTile = nullptr;
	int closestDistance = MAX_int32;

	for (int i = 0; i < tile.GetNeighbors().Num(); i++)
	{
		AGridTile* tileToCheck = tile.GetNeighbors()[i];

		if (checkedTiles.Contains(tileToCheck))
			continue;

		checkedTiles.Add(tileToCheck);

		int distance = CalculateDistanceToTarget(*tileToCheck, destination);

		if (distance > closestDistance)
			continue;

		closestDistance = distance;
		closestTile = tileToCheck;
	}

	return closestTile;
}

int AGridPathfinding::CalculateDistanceToTarget(const AGridTile& tile, const AGridTile& target)
{
	int cost = FMath::Max(FMath::Abs(tile.GetGridIndex().X - target.GetGridIndex().X), FMath::Abs(tile.GetGridIndex().Y - target.GetGridIndex().Y));
	return cost;
}

void AGridPathfinding::ShowPath(TArray<AGridTile*> path)
{
	//clear prev path
	for (int i = 0; i < _visiblePath.Num(); i++)
	{
		_grid->ResetTileVisual(_visiblePath[i]);
	}

	_visiblePath.Empty();
	_visiblePath = path;

	for (int i = 0; i < _visiblePath.Num(); i++)
	{
		_grid->SetTileReachable(_visiblePath[i]);
	}
}



