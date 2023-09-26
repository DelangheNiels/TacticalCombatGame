// Fill out your copyright notice in the Description page of Project Settings.


#include "GridObstacleVolume.h"

// Sets default values
AGridObstacleVolume::AGridObstacleVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	_rootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRootComponent"));
	SetRootComponent(_rootComponent);

	_staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	_staticMesh->SetupAttachment(_rootComponent);

}

// Called when the game starts or when spawned
void AGridObstacleVolume::BeginPlay()
{
	Super::BeginPlay();
	
	//Only show mesh for debug visuals
	_staticMesh->SetVisibility(false);
}


