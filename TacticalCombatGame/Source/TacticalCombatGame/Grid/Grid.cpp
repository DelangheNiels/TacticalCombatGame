// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"

#include "Components/InstancedStaticMeshComponent.h"

// Sets default values
AGrid::AGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_rootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneComponent"));
	SetRootComponent(_rootComponent);

	_instancedGridMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("GridMeshInstance"));
	_instancedGridMesh->SetupAttachment(_rootComponent);

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

	_instancedGridMesh->SetStaticMesh(_tileData.FlatMesh);
	_instancedGridMesh->SetMaterial(0, _tileData.BorderMaterial);

	FVector bottomLeftCornerPosition = CalculateBottomLeftPosition();

	for (int row = 0; row < _rowColumnSize.X; row++)
	{
		for (int column = 0; column < _rowColumnSize.Y; column++)
		{
			FVector tileLocation = bottomLeftCornerPosition + FVector(_tileSize.X * row, _tileSize.Y * column, 0);

			if (!_shouldScanEnvironment)
				SpawnTile(tileLocation);
			else
				TraceCheck(tileLocation);
		}
	}
}

void AGrid::DestroyGrid()
{
	_instancedGridMesh->ClearInstances();
}

FVector AGrid::CalculateCenterPosition()
{
	return SnapVectorToVector(GetActorLocation(), _tileSize);
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

	FVector2D moveDistance = (gridSize / 2) * FVector2D(_tileSize);
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

void AGrid::SpawnTile(const FVector& tileLocation)
{
	FVector tileScale = _tileSize / _tileData.MeshSize;
	FTransform tileTransform = FTransform(FRotator::ZeroRotator, tileLocation, tileScale);
	_instancedGridMesh->AddInstanceWorldSpace(tileTransform);
}

void AGrid::TraceCheck(const FVector& position)
{
	FHitResult hit;
	//Start trace high up and end low to make sure we hit something
	FVector traceStart = position + FVector(0,0,1000);
	FVector traceEnd = position + FVector(0, 0, -1000);
	FCollisionQueryParams queryParams;
	queryParams.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(hit, traceStart, traceEnd, _traceChannel, queryParams);

	if (!hit.bBlockingHit || !IsValid(hit.GetActor()))
		return; //if nothing hit, return

	//if hit something, spawn tile at location
	FVector hitLocation = hit.Location;
	hitLocation += FVector(0, 0, _tileHeightOffset); //add small offset in height to stop Z-fighting
	SpawnTile(hitLocation);

}


