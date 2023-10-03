// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridPathfinding.generated.h"

class AGridTile;
class AGrid;

UCLASS()
class TACTICALCOMBATGAME_API AGridPathfinding : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridPathfinding();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY()
		AGrid* _grid;

	//Functions

	void SetNeighborsForTiles();

	AGridTile* FindTileByIndex(const FVector2D& index);

};
