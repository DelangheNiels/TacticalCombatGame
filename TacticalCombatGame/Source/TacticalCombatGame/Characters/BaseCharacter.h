// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseCharacter.generated.h"

class AGridTile;
class AGrid;
class AGridPathfinding;
class UHealthComponent;

UCLASS()
class TACTICALCOMBATGAME_API ABaseCharacter : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	AGridTile* GetCurrentTile() const;
	void SetCurrentTile(AGridTile* tile);

	void ShowReachableTiles();
	void HideReachableTiles();

	void OnSelected();
	void CreatePathToDestination(const AGridTile& destination);
	void MoveToDestination();

	void ClearVisuals();

	UHealthComponent* GetHealthComponent() const;


private:

	UPROPERTY()
		USceneComponent* _rootComponent;

	UPROPERTY(EditInstanceOnly, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* _mesh;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		int _totalAmountOfTilesCharCanWalk = 4;
	
	UPROPERTY()
		AGridTile* _currentTile;
	UPROPERTY()
		AGridTile* _destinationTile;

	UPROPERTY()
		AGrid* _grid;
	UPROPERTY()
		AGridPathfinding* _pathfinding;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCharacterHUD> _characterHudRef;
	UPROPERTY()
		UCharacterHUD* _characterHud;

	UPROPERTY()
		TArray<AGridTile*> _reachableTiles;

	UPROPERTY(EditInstanceOnly, meta = (AllowPrivateAccess = "true"))
		UHealthComponent* _healthComponent;

	// Functions

	void FindReachableTiles();

	bool IsTileReachable(const AGridTile& tile);


};
