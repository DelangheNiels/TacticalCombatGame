// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GridMovementComponent.generated.h"

class AGridTile;
class AGrid;
class AGridPathfinding;

DECLARE_MULTICAST_DELEGATE(FOnCharacterMoved);
DECLARE_MULTICAST_DELEGATE(FOnCharacterStopedMoving);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TACTICALCOMBATGAME_API UGridMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGridMovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	AGridTile* GetCurrentTile() const;
	void SetCurrentTile(AGridTile* tile);

	int GetTotalAmountOfMovement() const;
	int GetCurrentAmountOfMovement() const;

	void CreatePathToDestination(const AGridTile& destination);
	void MoveToDestination();

	void ShowReachableTiles();
	void HideReachableTiles();

	void ClearVisuals();
	void Reset();

	FOnCharacterMoved OnCharacterMoved;
	FOnCharacterStopedMoving OnCharacterStopedMoving;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Movement")
		int _totalAmountOfTilesCharCanWalk = 4;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Movement")
		float _movementSpeed = 10.0f;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Movement")
		float _distanceThreshold = 0.0001f;

	int _currentAmountOfTilesCharCanWalk;
	int _currentPathSize;

	bool _isMoving;

	UPROPERTY()
		AGridTile* _currentTile;
	UPROPERTY()
		AGridTile* _destinationTile;

	UPROPERTY()
		AGrid* _grid;
	UPROPERTY()
		AGridPathfinding* _pathfinding;

	UPROPERTY()
		TArray<AGridTile*> _reachableTiles;

	UPROPERTY()
		TArray<AGridTile*> _pathToFollow;


	// Functions

	void FindReachableTiles();

	bool IsTileReachable(const AGridTile& tile);

	void MoveAlongPath();

	void SetCorrectRotation(const FVector& playerLocation, const FVector& targetLocation);

		
};
