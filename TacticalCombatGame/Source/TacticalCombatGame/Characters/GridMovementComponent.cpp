// Fill out your copyright notice in the Description page of Project Settings.


#include "GridMovementComponent.h"

#include "../Grid/GridTile.h"
#include "../Grid/Grid.h"
#include "../Pathfinding/GridPathfinding.h"

#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UGridMovementComponent::UGridMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGridMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	
	_grid = Cast<AGrid>(UGameplayStatics::GetActorOfClass(GetWorld(), AGrid::StaticClass()));
	_pathfinding = Cast<AGridPathfinding>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridPathfinding::StaticClass()));

	_currentAmountOfTilesCharCanWalk = _totalAmountOfTilesCharCanWalk;
}


// Called every frame
void UGridMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!_isMoving)
		return;

	MoveAlongPath();
}

AGridTile* UGridMovementComponent::GetCurrentTile() const
{
	return _currentTile;
}

void UGridMovementComponent::SetCurrentTile(AGridTile* tile)
{
	if (!tile)
		return;

	_currentTile = tile;
}

int UGridMovementComponent::GetTotalAmountOfMovement() const
{
	return _totalAmountOfTilesCharCanWalk;
}

int UGridMovementComponent::GetCurrentAmountOfMovement() const
{
	return _currentAmountOfTilesCharCanWalk;
}

void UGridMovementComponent::CreatePathToDestination(const AGridTile& destination)
{
	if (!IsTileReachable(destination) || _currentAmountOfTilesCharCanWalk < 1)
		return;

	HideReachableTiles();

	//create path
	auto path = _pathfinding->GeneratePath(_currentTile, const_cast<AGridTile*>(&destination));
	if (path.Num() <= 0)
		return;

	_currentPathSize = path.Num() - 1;
	_destinationTile = const_cast<AGridTile*>(&destination);
	_pathToFollow = path;
}

void UGridMovementComponent::MoveToDestination()
{
	if (_destinationTile == nullptr)
		return;

	_currentTile->SetIsCharacterOnTile(false);

	_currentTile = _destinationTile;
	_currentTile->SetIsCharacterOnTile(true);

	_currentAmountOfTilesCharCanWalk -= _currentPathSize;
	_currentPathSize = 0;

	_destinationTile = nullptr;

	OnCharacterMoved.Broadcast();
	_isMoving = true;

	SetCorrectRotation(GetOwner()->GetActorLocation(), _pathToFollow[0]->GetActorLocation());
}

void UGridMovementComponent::ShowReachableTiles()
{
	FindReachableTiles();

	for (int i = 0; i < _reachableTiles.Num(); i++)
	{
		_grid->SetTileReachable(_reachableTiles[i]);
	}
}

void UGridMovementComponent::HideReachableTiles()
{
	for (int i = 0; i < _reachableTiles.Num(); i++)
	{
		_grid->ResetTileVisual(_reachableTiles[i]);
	}
}

TArray<AGridTile*> UGridMovementComponent::GetReachableTiles()
{
	FindReachableTiles();
	return _reachableTiles;
}

void UGridMovementComponent::ClearVisuals()
{
	_grid->ResetGridVisuals();
}

void UGridMovementComponent::Reset()
{
	_currentAmountOfTilesCharCanWalk = _totalAmountOfTilesCharCanWalk;
	OnCharacterMoved.Broadcast();
}

void UGridMovementComponent::FindReachableTiles()
{
	_reachableTiles.Empty();

	TArray<AGridTile*> newTiles;
	_reachableTiles.Add(_currentTile);

	for (int i = 0; i < _currentAmountOfTilesCharCanWalk; i++)
	{
		for (int j = 0; j < _reachableTiles.Num(); j++)
		{
			TArray<AGridTile*> neighbors = _reachableTiles[j]->GetNeighbors();

			for (int k = 0; k < neighbors.Num(); k++)
			{
				if (_reachableTiles.Contains(neighbors[k]) || neighbors[k]->GetIsCharacterStandingOnTile())
					continue;
				newTiles.Add(neighbors[k]);
			}
		}

		_reachableTiles += newTiles;
	}
}

bool UGridMovementComponent::IsTileReachable(const AGridTile& tile)
{
	if (&tile == nullptr || !_reachableTiles.Contains(&tile))
		return false;

	return true;
}

void UGridMovementComponent::MoveAlongPath()
{
	if (_pathToFollow.Num() <= 0 || !_isMoving)
		return;

	FVector currentLocation = GetOwner()->GetActorLocation();
	const FVector destinationLocation = _pathToFollow[0]->GetActorLocation();


	FVector direction = destinationLocation - currentLocation;
	direction.Normalize();

	direction.Z = 0.0f;
	GetOwner()->SetActorLocation(currentLocation + (direction * _movementSpeed));

	currentLocation = GetOwner()->GetActorLocation();
	const float distance = FVector::Distance(currentLocation, destinationLocation);

	if (distance > _distanceThreshold)
		return;

	if (_pathToFollow.Num() > 0)
	{
		_pathToFollow.RemoveAt(0);

		if (_pathToFollow.Num() <= 0)
		{
			_isMoving = false;
			OnCharacterStopedMoving.Broadcast(this);

			GetOwner()->SetActorLocation(_currentTile->GetActorLocation());
		}
		else
		{
			SetCorrectRotation(currentLocation, _pathToFollow[0]->GetActorLocation());
		}
	}
	
}

void UGridMovementComponent::SetCorrectRotation(const FVector& playerLocation, const FVector& targetLocation)
{
	const FRotator playerRotation = UKismetMathLibrary::FindLookAtRotation(playerLocation, targetLocation);

	GetOwner()->SetActorRotation(playerRotation);
}

