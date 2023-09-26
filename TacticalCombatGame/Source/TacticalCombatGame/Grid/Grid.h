// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grid.generated.h"

USTRUCT(BlueprintType)
struct FGridTileData
{
	GENERATED_BODY()

public:
		UPROPERTY(EditAnywhere)
			FVector MeshSize;
		UPROPERTY(EditAnywhere)
			UStaticMesh* Mesh;
		UPROPERTY(EditAnywhere)
			UMaterialInstance* MeshMaterial;
		UPROPERTY(EditAnywhere)
			UStaticMesh* FlatMesh;
		UPROPERTY(EditAnywhere)
			UMaterialInstance* BorderMaterial;
		UPROPERTY(EditAnywhere)
			UMaterialInstance* FillMaterial;

};

UCLASS()
class TACTICALCOMBATGAME_API AGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrid();

	virtual void OnConstruction(const FTransform& transform) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(EditInstanceOnly)
		USceneComponent* _rootComponent;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		FGridTileData _tileData;

	UPROPERTY(EditInstanceOnly, meta = (AllowPrivateAccess = "true"))
		class UInstancedStaticMeshComponent* _instancedGridMesh;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		FVector _tileSize;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		FVector2D _rowColumnSize;

	UPROPERTY(EditAnywhere)
		TEnumAsByte<ECollisionChannel> _traceChannel;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		float _tileHeightOffset = 0.05f;

	UPROPERTY(EditAnywhere)
		bool _shouldScanEnvironment; //if true, only place grid tiles on ground 

	//Functions

	void SpawnGrid();

	void DestroyGrid();

	FVector CalculateCenterPosition();
	FVector CalculateBottomLeftPosition();

	//Used to allign grid with template lines
	FVector SnapVectorToVector(const FVector& v1, const FVector& v2);

	void SpawnTile(const FVector& tileLocation);

	void TraceCheck(const FVector& position);
};
