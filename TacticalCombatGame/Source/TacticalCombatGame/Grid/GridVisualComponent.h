// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GridVisualComponent.generated.h"

class AGridTile;

USTRUCT(BlueprintType)
struct FGridTileData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		FVector MeshSize;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AGridTile> GridTileClass;
	UPROPERTY(EditAnywhere)
		UMaterialInstance* MeshMaterial;
	UPROPERTY(EditAnywhere)
		UStaticMesh* FlatMesh;
	UPROPERTY(EditAnywhere)
		UMaterialInstance* BorderMaterial;
	UPROPERTY(EditAnywhere)
		UMaterialInstance* FillMaterial;

};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TACTICALCOMBATGAME_API UGridVisualComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGridVisualComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	AGridTile* CreateTile(const FVector& tileLocation);

	FVector GetTileSize() const;

private:

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		FGridTileData _tileData;

	UPROPERTY(EditAnywhere)
		TEnumAsByte<ECollisionChannel> _groundTraceChannel;
	UPROPERTY(EditAnywhere)
		TEnumAsByte<ETraceTypeQuery> _gridTraceChannel;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		float _tileHeightOffset = 0.05f;

	UPROPERTY(EditAnywhere)
		bool _shouldScanEnvironment; //if true, only place grid tiles on ground 

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		FVector _tileSize;

	//Functions

	AGridTile* SpawnTile(const FVector& tileLocation);

	AGridTile* TraceCheck(const FVector& position);

	bool HasHitObstacle(const FHitResult& hitResult);
};
