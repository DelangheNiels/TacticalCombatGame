// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CharacterSpawner.generated.h"

class ABaseCharacter;
class AGridTile;

UCLASS()
class TACTICALCOMBATGAME_API ACharacterSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACharacterSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:

	UPROPERTY()
		USceneComponent* _rootComponent;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<ABaseCharacter> _characterToSpawn;

	UPROPERTY(EditAnywhere)
		TEnumAsByte<ECollisionChannel> _gridTraceChannel;

	//Functions
	void SpawnCharacter();

	AGridTile* GetTileToSpawnPlayerOn();
};
