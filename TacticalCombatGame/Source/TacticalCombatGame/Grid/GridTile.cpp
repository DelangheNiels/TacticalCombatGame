// Fill out your copyright notice in the Description page of Project Settings.


#include "GridTile.h"

#include "Components/TextRenderComponent.h"

// Sets default values
AGridTile::AGridTile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	_rootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRootComponent"));
	SetRootComponent(_rootComponent);

	_staticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticmeshComponent"));
	_staticMeshComp->SetupAttachment(_rootComponent);

	_debugText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextrenderComponent"));
	_debugText->SetupAttachment(_rootComponent);
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

void AGridTile::SetGridIndex(const FVector2D& gridIndex)
{
	_gridIndex = gridIndex;

	FString text = FString::Printf(TEXT("%i , %i"), (int)_gridIndex.X, (int)_gridIndex.Y);
	_debugText->SetText(FText::FromString(text));
}

void AGridTile::AddNeighbor(AGridTile* tile)
{
	if (tile == nullptr ||_neighbors.Contains(tile))
		return;

	_neighbors.Add(tile);
}

// X value is column index
// Y value is row index
FVector2D AGridTile::GetGridIndex() const
{
	return _gridIndex;
}

TArray<AGridTile*> AGridTile::GetNeighbors() const
{
	return _neighbors;
}

bool AGridTile::GetIsCharacterStandingOnTile() const
{
	return _hasCharacterOnTile;
}

void AGridTile::SetIsCharacterOnTile(bool isOnTile)
{
	_hasCharacterOnTile = isOnTile;
}


