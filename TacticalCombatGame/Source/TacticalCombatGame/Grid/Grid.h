// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grid.generated.h"

UCLASS()
class TACTICALCOMBATGAME_API AGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrid();

	virtual void OnConstruction(const FTransform& transform) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SelectTile();

private:

	UPROPERTY()
		USceneComponent* _rootComponent;

	UPROPERTY(EditInstanceOnly)
		class UGridVisualComponent* _gridVisualComp;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		FVector2D _rowColumnSize;

	UPROPERTY()
		TMap<FVector,class AGridTile*> _gridTileMap;

	UPROPERTY()
		APlayerController* _playerController;

	//Functions

	void SpawnGrid();

	void DestroyGrid();

	FVector CalculateCenterPosition();
	FVector CalculateBottomLeftPosition();

	//Used to allign grid with template lines
	FVector SnapVectorToVector(const FVector& v1, const FVector& v2);

	void CheckTileHover();

	AGridTile* GetTileByLocation(const FVector& location);

};
