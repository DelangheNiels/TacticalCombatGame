// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyPlayer.generated.h"

class ABaseCharacter;
class UHealthComponent;
class ATacticalCombatGameGameModeBase;

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

	UPROPERTY()
		TArray<ABaseCharacter*> _charactersToControl;
	UPROPERTY()
		TArray<ABaseCharacter*> _controlledCharacters;

	UPROPERTY()
		FTimerHandle _controlCharactersTimer;

	UPROPERTY()
		ATacticalCombatGameGameModeBase* _gamemode;

	//Functions

	void GetAllCharactersToControl();

	UFUNCTION()
		void OnControlledCharacterDie(UHealthComponent* healthcomp);

	UFUNCTION()
		void ControlNextCharacter();

};
