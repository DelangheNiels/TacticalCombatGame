// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"
#include "GridObstacleVolume.h"
#include "GridVisualComponent.h"
#include "GridTile.h"

#include "Components/InstancedStaticMeshComponent.h"

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

	SpawnGrid();
	
}

// Called every frame
void AGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
			if (tile)
				_gridTiles.Add(tile);
		}
	}

	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("Amount of tiles in list: %i"), _gridTiles.Num()));
}

void AGrid::DestroyGrid()
{
	for (int i = 0; i < _gridTiles.Num(); i++)
	{
		_gridTiles[i]->Destroy();
	}

	_gridTiles.Empty();
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




