// Fill out your copyright notice in the Description page of Project Settings.


#include "GridTile.h"

// Sets default values
AGridTile::AGridTile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	_rootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRootComponent"));
	SetRootComponent(_rootComponent);

	_staticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticmeshComponent"));
	_staticMeshComp->SetupAttachment(_rootComponent);
}

// Called when the game starts or when spawned
void AGridTile::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AGridTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGridTile::SetMaterial(UMaterialInstance* material)
{
	_staticMeshComp->SetMaterial(0, material);
}


