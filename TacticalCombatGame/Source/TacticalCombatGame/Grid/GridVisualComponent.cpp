// Fill out your copyright notice in the Description page of Project Settings.


#include "GridVisualComponent.h"
#include "GridTile.h"
#include "GridObstacleVolume.h"

// Sets default values for this component's properties
UGridVisualComponent::UGridVisualComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGridVisualComponent::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void UGridVisualComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

AGridTile* UGridVisualComponent::CreateTile(const FVector& tileLocation)
{
	AGridTile* tile = nullptr;

	if (!_shouldScanEnvironment)
		tile = SpawnTile(tileLocation);
	else
		tile = TraceCheck(tileLocation);

	return tile;
}

FVector UGridVisualComponent::GetTileSize() const
{
	return _tileSize;
}

ECollisionChannel UGridVisualComponent::GetGridCollisionChannel() const
{
	return _gridTraceChannel;
}

void UGridVisualComponent::SetTileHoverVisual(AGridTile* tile)
{
	if (_hoverdTile == tile)
		return;

	if (tile == nullptr && _hoverdTile)
	{
		_hoverdTile->SetMaterial(_tileData.DefaultMaterial);
		_hoverdTile = nullptr;

		return;
	}

	if(tile != _selectedTile)
		tile->SetMaterial(_tileData.HoveredMaterial);

	//Set previous hoverd tile material back to default material
	if (_hoverdTile && _hoverdTile != _selectedTile)
		_hoverdTile->SetMaterial(_tileData.DefaultMaterial);

	_hoverdTile = tile;

}

void UGridVisualComponent::SetTileSelectedVisual(AGridTile* tile)
{
	if (!tile && !_selectedTile)
		return;

	//unselect tile when cliking on it again
	if (tile == _selectedTile && tile != nullptr)
	{
		_selectedTile->SetMaterial(_tileData.HoveredMaterial);
		_selectedTile = nullptr;

		return;
	}

	//unselect prev tile 
	if (_selectedTile)
	{
		_selectedTile->SetMaterial(_tileData.DefaultMaterial);
		_selectedTile = nullptr;
	}

	if (!tile)
		return;
	
	tile->SetMaterial(_tileData.SelectedMaterial);
	_selectedTile = tile;
}

void UGridVisualComponent::SetReachableVisual(AGridTile* tile)
{
	tile->SetMaterial(_tileData.HoveredMaterial);
}

void UGridVisualComponent::ResetVisual(AGridTile* tile)
{
	tile->SetMaterial(_tileData.DefaultMaterial);
}

void UGridVisualComponent::ClearNeighbors()
{
	for (int i = 0; i < _visibleNeighbors.Num(); i++)
	{
		_visibleNeighbors[i]->SetMaterial(_tileData.DefaultMaterial);
	}
	_visibleNeighbors.Empty();
}

//show neighbors for this tile
void UGridVisualComponent::SetNeighborVisuals(AGridTile* tile)
{
	ClearNeighbors();

	if (_selectedTile == nullptr)
		return;

	for (int i = 0; i < tile->GetNeighbors().Num(); i++)
	{
		tile->GetNeighbors()[i]->SetMaterial(_tileData.HoveredMaterial);
		_visibleNeighbors.Add(tile->GetNeighbors()[i]);
	}
}

AGridTile* UGridVisualComponent::SpawnTile(const FVector& tileLocation)
{
	FVector tileScale = _tileSize / _tileData.MeshSize;
	FTransform tileTransform = FTransform(FRotator::ZeroRotator, tileLocation, tileScale);

	auto tile = GetWorld()->SpawnActor<AGridTile>(_tileData.GridTileClass, tileTransform);
	return tile;
}

AGridTile* UGridVisualComponent::TraceCheck(const FVector& position)
{
	FHitResult hit;
	//Start trace high up and end low to make sure we hit something
	FVector traceStart = position + FVector(0, 0, 1000);
	FVector traceEnd = position + FVector(0, 0, -1000);
	FCollisionQueryParams queryParams;
	queryParams.AddIgnoredActor(GetOwner());
	GetWorld()->LineTraceSingleByChannel(hit, traceStart, traceEnd, _groundTraceChannel, queryParams);

	if (!hit.bBlockingHit || !IsValid(hit.GetActor()) || HasHitObstacle(hit))
		return nullptr; //do not spawn tile when hitting nothing or obstacle

	//if hit something, spawn tile at location
	FVector hitLocation = hit.Location;
	hitLocation += FVector(0, 0, _tileHeightOffset); //add small offset in height to stop Z-fighting
	return SpawnTile(hitLocation);
}

bool UGridVisualComponent::HasHitObstacle(const FHitResult& hitResult)
{
	AGridObstacleVolume* obstacleVolume = Cast<AGridObstacleVolume>(hitResult.GetActor());
	if (obstacleVolume)
		return true;

	return false;
}

