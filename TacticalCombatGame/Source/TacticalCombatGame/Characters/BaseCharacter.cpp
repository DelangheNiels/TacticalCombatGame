// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"

#include "../Grid/GridTile.h"
#include "../Grid/Grid.h"
#include "../Pathfinding/GridPathfinding.h"

#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_rootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRootComponent"));
	SetRootComponent(_rootComponent);

	_mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	_mesh->SetupAttachment(_rootComponent);
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	_grid = Cast<AGrid>(UGameplayStatics::GetActorOfClass(GetWorld(), AGrid::StaticClass()));
	_pathfinding = Cast<AGridPathfinding>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridPathfinding::StaticClass()));
	
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AGridTile* ABaseCharacter::GetCurrentTile() const
{
	return _currentTile;
}

void ABaseCharacter::SetCurrentTile(AGridTile* tile)
{
	_currentTile = tile;
	ShowReachableTiles();
}

void ABaseCharacter::ShowReachableTiles()
{
	TArray<AGridTile*> reachableTiles;
	TArray<AGridTile*> newTiles;
	reachableTiles.Add(_currentTile);
	
	for (int i = 0; i < _totalAmountOfTilesCharCanWalk; i++)
	{
		for (int j = 0; j < reachableTiles.Num(); j++)
		{
			TArray<AGridTile*> neighbors = reachableTiles[j]->GetNeighbors();
			
			for (int k = 0; k < neighbors.Num(); k++)
			{
				if (reachableTiles.Contains(neighbors[k]))
					continue;
				newTiles.Add(neighbors[k]);
			}
		}

		reachableTiles += newTiles;
	}

	for (int i = 0; i < reachableTiles.Num(); i++)
	{
		_grid->SetTileReachable(reachableTiles[i]);
	}
}

