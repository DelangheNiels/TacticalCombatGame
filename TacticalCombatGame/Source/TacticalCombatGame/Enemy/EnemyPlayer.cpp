// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyPlayer.h"

#include "../Characters/BaseCharacter.h"
#include "../Characters/HealthComponent.h"
#include "../Characters/GridMovementComponent.h"
#include "../Characters/Attacks/AttackComponent.h"

#include "../Grid/GridTile.h"

#include "../TacticalCombatGameGameModeBase.h"

#include "Kismet/GameplayStatics.h"

#include "Math/NumericLimits.h"
#include <Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>

// Sets default values
AEnemyPlayer::AEnemyPlayer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyPlayer::BeginPlay()
{
	Super::BeginPlay();

	GetAllCharactersToControl();

	_gamemode = Cast<ATacticalCombatGameGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	if (!_gamemode)
		return;

	//let game mode know that enemy player is spawned in
	_gamemode->CreateEnemyPlayerStates(this);


	
}

// Called every frame
void AEnemyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyPlayer::ControlCharacters()
{
	GetWorld()->GetTimerManager().SetTimer(_controlCharactersTimer, this, &AEnemyPlayer::ControlNextCharacter, _waitTimeBetweenCharacters, true);
}

void AEnemyPlayer::ResetControlledCharacters()
{
	_charactersToControl = _controlledCharacters;
	_controlledCharacters.Empty();

	for (int i = 0; i < _charactersToControl.Num(); i++)
	{
		_charactersToControl[i]->Reset();
	}
}

void AEnemyPlayer::GetAllCharactersToControl()
{
	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseCharacter::StaticClass(), foundActors);

	if (foundActors.Num() <= 0)
		return;

	for (int i = 0; i < foundActors.Num(); i++)
	{
		auto character = Cast<ABaseCharacter>(foundActors[i]);

		if (!character->GetIsControlledByPlayer())
		{
			_charactersToControl.Add(character);
			character->GetHealthComponent()->OnDied.AddUObject(this, &AEnemyPlayer::OnControlledCharacterDie);
		}
		else
		{
			_opponentCharacters.Add(character);
			character->GetHealthComponent()->OnDied.AddUObject(this, &AEnemyPlayer::OnOpponentCharacterDied);
		}
	}
}

void AEnemyPlayer::OnControlledCharacterDie(UHealthComponent* healthcomp)
{
	healthcomp->OnDied.RemoveAll(this);

	auto character = Cast<ABaseCharacter>(healthcomp->GetOwner());

	if (!character)
		return;

	_charactersToControl.Remove(character);
}

void AEnemyPlayer::OnOpponentCharacterDied(UHealthComponent* healthcomp)
{
	healthcomp->OnDied.RemoveAll(this);

	auto character = Cast<ABaseCharacter>(healthcomp->GetOwner());

	if (!character)
		return;

	_opponentCharacters.Remove(character);
}

void AEnemyPlayer::ControlNextCharacter()
{
	if (_charactersToControl.Num() <= 0)
	{
		if (_gamemode)
			_gamemode->ChangeTurn();
		
		GetWorld()->GetTimerManager().ClearTimer(_controlCharactersTimer);
		return;
	}

	ABaseCharacter* character = _charactersToControl[0];

	auto closestOpponentTile = FindTileClosestOpponentTileToControlledChar(character);
	if (!closestOpponentTile)
		return;

	if (!character->GetAttackComponent()->GetAttackableTiles(character->GetGridMovementComponent()->GetCurrentTile()).Contains(closestOpponentTile))
	{
		TryMovingCharacter(character, closestOpponentTile);
	}
	else
	{
		auto closestOpponent = FindClosestOpponentToControlledChar(character);
		DealDamage(character, closestOpponent);
	}

	_charactersToControl.Remove(character);
	_controlledCharacters.Add(character);
}

AGridTile* AEnemyPlayer::FindTileClosestOpponentTileToControlledChar(ABaseCharacter* character) const
{
	auto closestChar = FindClosestOpponentToControlledChar(character);
	if(!closestChar)
		return nullptr;

	return closestChar->GetGridMovementComponent()->GetCurrentTile();
}

ABaseCharacter* AEnemyPlayer::FindClosestOpponentToControlledChar(ABaseCharacter* character) const
{
	if (_opponentCharacters.Num() <= 0)
		return nullptr;

	float closestDistance = TNumericLimits<float>::Max();
	ABaseCharacter* closestCharacter = nullptr;

	for (int i = 0; i < _opponentCharacters.Num(); i++)
	{
		float distance = FVector::Distance(character->GetActorLocation(), _opponentCharacters[i]->GetGridMovementComponent()->GetCurrentTile()->GetActorLocation());
		if (distance <= closestDistance)
		{
			closestDistance = distance;
			closestCharacter = _opponentCharacters[i];
		}
	}

	return closestCharacter;
}

bool AEnemyPlayer::IsOpponentTileInAttackRange(AGridTile* opponentTile, ABaseCharacter* character,AGridTile*& tileToAttackOpponent)
{
	bool result = false;	
	TArray<AGridTile*> reachableTiles = character->GetGridMovementComponent()->GetReachableTiles();
	TArray<AGridTile*> attackableTiles;

	//look at each tile and see what tiles it can attack
	for (int i = 0; i < reachableTiles.Num(); i++)
	{
		attackableTiles = character->GetAttackComponent()->GetAttackableTiles(reachableTiles[i]);

		for (int j = 0; j < attackableTiles.Num(); j++)
		{
			if (attackableTiles[j] == opponentTile)
			{
				tileToAttackOpponent = reachableTiles[i];
				result = true;
				break;
			}
		}
	}

	return result;
}

AGridTile* AEnemyPlayer::GetClosestReachableTileToOpponentTile(ABaseCharacter* character, AGridTile* opponentTile)
{
	TArray<AGridTile*> reachableTiles = character->GetGridMovementComponent()->GetReachableTiles();

	AGridTile* closestTile = nullptr;

	float closestDistance = TNumericLimits<float>::Max();

	for (int i = 0; i < reachableTiles.Num(); i++)
	{
		float distance = FVector::Distance(reachableTiles[i]->GetActorLocation(), opponentTile->GetActorLocation());

		if (distance < closestDistance)
		{
			closestTile = reachableTiles[i];
			closestDistance = distance;
		}
	}

	return closestTile;
}

void AEnemyPlayer::MoveToOpponent(ABaseCharacter* character, AGridTile* tile)
{
	character->GetGridMovementComponent()->CreatePathToDestination(*tile);

	FTimerDelegate moveCharacterDelegate;
	moveCharacterDelegate.BindUFunction(this, FName("MoveCharacter"), character);
	GetWorld()->GetTimerManager().SetTimer(_moveToDestinationTimer, moveCharacterDelegate, _waitTimeToMove, false);
}

void AEnemyPlayer::MoveCharacter(ABaseCharacter* character)
{
	character->GetGridMovementComponent()->ClearVisuals();
	GetWorld()->GetTimerManager().ClearTimer(_moveToDestinationTimer);
	character->GetGridMovementComponent()->MoveToDestination();
}

void AEnemyPlayer::OnCharacterStoppedMoving(UGridMovementComponent* movementComp)
{
	movementComp->OnCharacterStopedMoving.RemoveAll(this);

	auto character = Cast<ABaseCharacter>(movementComp->GetOwner());

	auto closestOpponent = FindClosestOpponentToControlledChar(character);
	DealDamage(character, closestOpponent);
}

void AEnemyPlayer::TryMovingCharacter(ABaseCharacter* character, AGridTile* opponentTile)
{
	AGridTile* tile = nullptr;
	if (IsOpponentTileInAttackRange(opponentTile, character, tile))
	{
		//move to tile to attack enemy
		if (tile)
		{
			character->GetGridMovementComponent()->OnCharacterStopedMoving.AddUObject(this, &AEnemyPlayer::OnCharacterStoppedMoving);
			MoveToOpponent(character, tile);
		}
	}
	else
	{
		//move to tile closest to enemy 
		tile = GetClosestReachableTileToOpponentTile(character, opponentTile);
		if (!tile)
			return;

		MoveToOpponent(character, tile);

	}
}

void AEnemyPlayer::DealDamage(ABaseCharacter* character, ABaseCharacter* opponent)
{
	//Rotate towards opponent
	const FRotator playerRotation = UKismetMathLibrary::FindLookAtRotation(character->GetActorLocation(), opponent->GetActorLocation());
	character->SetActorRotation(playerRotation);

	//show visual tile to attack
	character->GetAttackComponent()->ShowTilesToAttack(character->GetGridMovementComponent()->GetCurrentTile()->GetGridIndex(), FVector2D(character->GetActorForwardVector()));

	//wait a short time to show tile to attak and deal damage to opponent character
	FTimerDelegate attackDelegate;
	attackDelegate.BindUFunction(this, FName("DoAttack"), character);
	GetWorld()->GetTimerManager().SetTimer(_attackTimer, attackDelegate, _waitTimeToAttack, false);

}

void AEnemyPlayer::DoAttack(ABaseCharacter* character)
{
	GetWorld()->GetTimerManager().ClearTimer(_attackTimer);
	character->GetGridMovementComponent()->ClearVisuals();
	character->GetAttackComponent()->DoAttack();
}



