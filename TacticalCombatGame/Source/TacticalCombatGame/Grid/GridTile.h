// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridTile.generated.h"

UCLASS()
class TACTICALCOMBATGAME_API AGridTile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGridTile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetMaterial(UMaterialInstance* material);

private:

	UPROPERTY(EditInstanceOnly)
		USceneComponent* _rootComponent;

	UPROPERTY(EditInstanceOnly)
		UStaticMeshComponent* _staticMeshComp;

};
