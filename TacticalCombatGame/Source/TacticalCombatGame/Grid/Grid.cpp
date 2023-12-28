// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"
#include "GridObstacleVolume.h"
#include "GridVisualComponent.h"
#include "GridTile.h"

#include "Components/InstancedStaticMeshComponent.h"

#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values
AGrid::AGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_rootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneComponent"));
	SetRootComponent(_rootComponent);

	_gridVisualComp = CreateDefaultSubobject<UGridVisualComponent>(TEXT("GridVisualComponent"));

}

void AGrid::OnConstruction(const FTransform& transform)
{
	Super::OnConstruction(transform);

	SpawnGrid();
}

// Called when the game starts or when spawned
void AGrid::BeginPlay()
{
	Super::BeginPlay();

	_playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	
}

// Called every frame
void AGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//CheckTileHover();

}

AGridTile* AGrid::SelectTile()
{
	FHitResult hit;
	_playerController->GetHitResultUnderCursor(_gridVisualComp->GetGridCollisionChannel(), false, hit);

	//unselect prev tile when clicking out of grid
	if (!hit.bBlockingHit)
	{
		_gridVisualComp->SetTileSelectedVisual(nullptr);
		return nullptr;
	}

	AGridTile* hoveredTile = GetTileByLocation(hit.GetActor()->GetActorLocation());
	_gridVisualComp->SetTileSelectedVisual(hoveredTile);

	//Used for debuging
	if (showNeighbors)
	{
		//show neighbors for this tile
		_gridVisualComp->SetNeighborVisuals(hoveredTile);
	}

	return hoveredTile;
		
}

void AGrid::SetTileReachable(AGridTile* tile)
{
	_gridVisualComp->SetReachableVisual(tile);
}

void AGrid::ResetTileVisual(AGridTile* tile)
{
	_gridVisualComp->ResetVisual(tile);
}

void AGrid::ResetGridVisuals()
{
	if (_tiles.Num() <= 0)
		return;

	for (int i = 0; i < _tiles.Num(); i++)
	{
		ResetTileVisual(_tiles[i]);
	}
}

TMap<FVector, AGridTile*> AGrid::GetTileLocationMap() const
{
	return _gridTileLocationMap;
}


void AGrid::SpawnGrid()
{
	DestroyGrid();
	FVector bottomLeftCornerPosition = CalculateBottomLeftPosition();

	for (int row = 0; row < _rowColumnSize.X; row++)
	{
		for (int column = 0; column < _rowColumnSize.Y; column++)
		{
			FVector tileLocation = bottomLeftCornerPosition + FVector(_gridVisualComp->GetTileSize().X * row, _gridVisualComp->GetTileSize().Y * column, 0);

			AGridTile* tile = _gridVisualComp->CreateTile(tileLocation);

			if (!tile)
				continue;

			FVector2D index = FVector2D(row, column);
			tile->SetGridIndex(index);
			_gridTileLocationMap.Add(tile->GetActorLocation(), tile);
			_tiles.Add(tile);
		}
	}

	SetNeighborsForTiles();
}

void AGrid::DestroyGrid()
{
	for (auto& element : _gridTileLocationMap)
	{
		element.Value->Destroy();
	}

	_tiles.Empty();
	_gridTileLocationMap.Empty();
}

FVector AGrid::CalculateCenterPosition()
{
	return SnapVectorToVector(GetActorLocation(), _gridVisualComp->GetTileSize());
}

FVector AGrid::CalculateBottomLeftPosition()
{
	FVector centerLocation = CalculateCenterPosition();

	FVector2D gridSize = _rowColumnSize;

	//Substract 1 tile when row or column amount is odd to get correct allignment
	if (int(_rowColumnSize.X) % 2 != 0)
		--gridSize.X;
	if (int(_rowColumnSize.Y) % 2 != 0)
		--gridSize.Y;

	FVector2D moveDistance = (gridSize / 2) * FVector2D(_gridVisualComp->GetTileSize());
	FVector bottomLeftCornerPosition = centerLocation - FVector(moveDistance, 0);
	return bottomLeftCornerPosition;
}


FVector AGrid::SnapVectorToVector(const FVector& v1, const FVector& v2)
{
	float x = FMath::GridSnap(v1.X, v2.X);
	float y = FMath::GridSnap(v1.Y, v2.Y);
	float z = FMath::GridSnap(v1.Z, v2.Z);

	return FVector(x,y,z);
}

void AGrid::CheckTileHover()
{
	FHitResult hit;
	_playerController->GetHitResultUnderCursor(_gridVisualComp->GetGridCollisionChannel(), false, hit);

	//Set prev hovered tile back to normal when moving out of grid
	if (!hit.bBlockingHit)
	{
		_gridVisualComp->SetTileHoverVisual(nullptr);
		return;
	}

	AGridTile* hoveredTile = GetTileByLocation(hit.GetActor()->GetActorLocation());
	_gridVisualComp->SetTileHoverVisual(hoveredTile);
}

AGridTile* AGrid::GetTileByLocation(const FVector& location)
{
	return *_gridTileLocationMap.Find(location);
}

void AGrid::SetNeighborsForTiles()
{
	for (int i = 0; i < _tiles.Num(); i++)
	{
		AGridTile* tile = _tiles[i];

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

AGridTile* AGrid::FindTileByIndex(const FVector2D& index)
{
	for (int i = 0; i < _tiles.Num(); i++)
	{
		if (_tiles[i]->GetGridIndex().Equals(index))
		{
			return _tiles[i];
		}	
	}

	return nullptr;
}