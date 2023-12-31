// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttackComponent.generated.h"

class UBaseAttack;
class AGridTile;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TACTICALCOMBATGAME_API UAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttackComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	void ShowTilesToAttack(const FVector2D& currentTileIndex, const FVector2D& characterForwardVector);
	void DoAttack();
	void HideTilesToAttack();
	bool GetWantsToAttack() const;
	void StopAttacking();
	bool GetHasAttacked() const;
	void Reset();
	int GetAttackRange() const;
	int GetAttackOffset() const;
	TArray<AGridTile*> GetAttackableTiles(AGridTile* currentTile);

private:

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		float _damage;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UBaseAttack> _attackClass;
	
	UPROPERTY()
		UBaseAttack* _attack;

	UPROPERTY()
		bool _wantsToAttack;

	bool _hasAttacked;
};
