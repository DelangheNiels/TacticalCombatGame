// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyPlayer.generated.h"

class ABaseCharacter;
class UHealthComponent;
class ATacticalCombatGameGameModeBase;
class AGridTile;
class APlayerPawn;
class UGridMovementComponent;

UCLASS()
class TACTICALCOMBATGAME_API AEnemyPlayer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ControlCharacters();

	void ResetControlledCharacters();

private:

	UPROPERTY(EditAnywhere, meta = (AllowrivateAccess = "true"))
		float _waitTimeBetweenCharacters = 0.7f;
	UPROPERTY(EditAnywhere, meta = (AllowrivateAccess = "true"))
		float _waitTimeToMove = 0.25f;
	UPROPERTY(EditAnywhere, meta = (AllowrivateAccess = "true"))
		float _waitTimeToAttack = 0.25f;

	UPROPERTY()
		TArray<ABaseCharacter*> _charactersToControl;
	UPROPERTY()
		TArray<ABaseCharacter*> _controlledCharacters;
	UPROPERTY()
		TArray<ABaseCharacter*> _opponentCharacters;

	UPROPERTY()
		FTimerHandle _controlCharactersTimer;
	UPROPERTY()
		FTimerHandle _moveToDestinationTimer;
	UPROPERTY()
		FTimerHandle _rotateCharTimer;
	UPROPERTY()
		FTimerHandle _attackTimer;

	UPROPERTY()
		ATacticalCombatGameGameModeBase* _gamemode;

	//Functions

	void GetAllCharactersToControl();

	UFUNCTION()
		void OnControlledCharacterDie(UHealthComponent* healthcomp);
	UFUNCTION()
		void OnOpponentCharacterDied(UHealthComponent* healthcomp);

	UFUNCTION()
		void ControlNextCharacter();

	UFUNCTION()
		AGridTile* FindTileClosestOpponentTileToControlledChar(ABaseCharacter* character) const;

	UFUNCTION()
		ABaseCharacter* FindClosestOpponentToControlledChar(ABaseCharacter* character) const;

	UFUNCTION()
		bool IsOpponentTileInAttackRange(AGridTile* opponentTile, ABaseCharacter* character,AGridTile*& tileToAttackOpponent);
	UFUNCTION()
		AGridTile* GetClosestReachableTileToOpponentTile(ABaseCharacter* character, AGridTile* opponentTile);

	UFUNCTION()
		void MoveToOpponent(ABaseCharacter* character, AGridTile* tile);
	UFUNCTION()
		void MoveCharacter(ABaseCharacter* character);

	UFUNCTION()
		void OnCharacterStoppedMoving(UGridMovementComponent* movementComp);

	void TryMovingCharacter(ABaseCharacter* character, AGridTile* tile);

	UFUNCTION()
		void DealDamage(ABaseCharacter* character, ABaseCharacter* opponent);

	UFUNCTION()
		void DoAttack(ABaseCharacter* character);
};
