// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "HealthComponent.h"

#include "Attacks/AttackComponent.h"

#include "../Grid/GridTile.h"
#include "../Grid/Grid.h"
#include "../Pathfinding/GridPathfinding.h"
#include "../UI/CharacterHUD.h"

#include "Blueprint/UserWidget.h"

#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_rootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRootComponent"));
	SetRootComponent(_rootComponent);

	_mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	_mesh->SetupAttachment(_rootComponent);

	_healthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	_attackComponent = CreateDefaultSubobject<UAttackComponent>(TEXT("AttackComponent"));
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	_grid = Cast<AGrid>(UGameplayStatics::GetActorOfClass(GetWorld(), AGrid::StaticClass()));
	_pathfinding = Cast<AGridPathfinding>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridPathfinding::StaticClass()));
	
	_currentAmountOfTilesCharCanWalk = _totalAmountOfTilesCharCanWalk;
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

void ABaseCharacter::ShowReachableTiles()
{
	FindReachableTiles();

	for (int i = 0; i < _reachableTiles.Num(); i++)
	{
		_grid->SetTileReachable(_reachableTiles[i]);
	}
}

void ABaseCharacter::HideReachableTiles()
{
	for (int i = 0; i < _reachableTiles.Num(); i++)
	{
		_grid->ResetTileVisual(_reachableTiles[i]);
	}
}

void ABaseCharacter::OnSelected()
{
	_characterHud = Cast<UCharacterHUD>(CreateWidget(GetWorld(), _characterHudRef));
	_characterHud->SetCharacter(this);
	_characterHud->AddToViewport();
}

void ABaseCharacter::OnDeselected()
{
	_attackComponent->StopAttacking();
	_characterHud->RemoveFromParent();
	_characterHud = nullptr;
}

void ABaseCharacter::CreatePathToDestination(const AGridTile& destination)
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
}

void ABaseCharacter::MoveToDestination()
{
	if (_destinationTile == nullptr)
		return;

	_currentTile->SetIsCharacterOnTile(false);

	SetActorLocation(_destinationTile->GetActorLocation());

	_currentTile = _destinationTile;
	_currentTile->SetIsCharacterOnTile(true);

	_currentAmountOfTilesCharCanWalk -= _currentPathSize;
	_currentPathSize = 0;

	_destinationTile = nullptr;

	OnCharacterMoved.Broadcast();
}

void ABaseCharacter::ClearVisuals()
{
	_grid->ResetGridVisuals();
}

UHealthComponent* ABaseCharacter::GetHealthComponent() const
{
	return _healthComponent;	
}

UAttackComponent* ABaseCharacter::GetAttackComponent() const
{
	return _attackComponent;
}

int ABaseCharacter::GetTotalAmountOfMovement() const
{
	return _totalAmountOfTilesCharCanWalk;
}

int ABaseCharacter::GetCurrentAmountOfMovement() const
{
	return _currentAmountOfTilesCharCanWalk;
}

void ABaseCharacter::FindReachableTiles()
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

bool ABaseCharacter::IsTileReachable(const AGridTile& tile)
{
	if(&tile == nullptr || !_reachableTiles.Contains(&tile))
		return false;

	return true;
}

