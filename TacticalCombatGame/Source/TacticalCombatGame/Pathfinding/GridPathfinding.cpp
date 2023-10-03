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



