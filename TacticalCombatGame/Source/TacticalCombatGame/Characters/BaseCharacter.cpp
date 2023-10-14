// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"

#include "../Grid/GridTile.h"

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
}

