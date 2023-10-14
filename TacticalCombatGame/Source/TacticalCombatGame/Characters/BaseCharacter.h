// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseCharacter.generated.h"

class AGridTile;

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

private:

	UPROPERTY()
		USceneComponent* _rootComponent;

	UPROPERTY(EditInstanceOnly, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* _mesh;

	UPROPERTY()
		AGridTile* _currentTile;

};
