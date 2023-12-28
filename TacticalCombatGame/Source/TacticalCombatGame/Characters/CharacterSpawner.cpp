// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSpawner.h"
#include "BaseCharacter.h"
#include "GridMovementComponent.h"

#include "../Grid/GridTile.h"

// Sets default values
ACharacterSpawner::ACharacterSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	_rootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneComponent"));
	SetRootComponent(_rootComponent);

}

// Called when the game starts or when spawned
void ACharacterSpawner::BeginPlay()
{
	Super::BeginPlay();

	SpawnCharacter();
	
}

void ACharacterSpawner::SpawnCharacter()
{
	//raycast to get tile
	auto tile = GetTileToSpawnPlayerOn();

	if (tile == nullptr)
		return;

	FRotator characterRotation = FRotator(0, GetActorRotation().Yaw, 0);

	auto character = GetWorld()->SpawnActor<ABaseCharacter>(_characterToSpawn, tile->GetActorLocation(), characterRotation);

	character->SetIsControlledByPlayer(_isControlledByPlayer);

	character->GetGridMovementComponent()->SetCurrentTile(tile);
	tile->SetIsCharacterOnTile(true);
}

AGridTile* ACharacterSpawner::GetTileToSpawnPlayerOn()
{
	FVector position = GetActorLocation();

	FHitResult hit;

	FVector traceStart = position + FVector(0, 0, 1000);
	FVector traceEnd = position + FVector(0, 0, -1000);
	FCollisionQueryParams queryParams;
	queryParams.AddIgnoredActor(GetOwner());
	GetWorld()->LineTraceSingleByChannel(hit, traceStart, traceEnd, _gridTraceChannel, queryParams);

	AGridTile* tile = Cast<AGridTile>(hit.GetActor());

	return tile;
}


