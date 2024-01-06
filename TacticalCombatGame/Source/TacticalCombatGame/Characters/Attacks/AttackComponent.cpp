// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackComponent.h"
#include "BaseAttack.h"

#include "../../Grid/Grid.h"
#include "../../Grid/GridTile.h"

#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UAttackComponent::UAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UAttackComponent::BeginPlay()
{
	Super::BeginPlay();	

	if (!_attackClass)
		return;

	_attack = NewObject<UBaseAttack>(this, _attackClass);
	if (!_attack)
		return;

	auto grid = Cast<AGrid>(UGameplayStatics::GetActorOfClass(GetWorld(), AGrid::StaticClass()));
	if (!grid)
		return;

	_attack->SetGrid(grid);
}

void UAttackComponent::ShowTilesToAttack(const FVector2D& currentTileIndex, const FVector2D& characterForwardVector)
{
	if (!_attack)
		return;

	_wantsToAttack = true;
	_attack->ShowTilesToAttack(currentTileIndex, characterForwardVector);
}

void UAttackComponent::DoAttack()
{
	if (!_attack)
		return;

	_wantsToAttack = false;
	_hasAttacked = true;
	_attack->Attack(_damage);
}

void UAttackComponent::HideTilesToAttack()
{
	if (!_attack)
		return;

	_wantsToAttack = false;
	_attack->HideTilesToAttack();
}

bool UAttackComponent::GetWantsToAttack() const
{
	return _wantsToAttack;
}

void UAttackComponent::StopAttacking()
{
	_wantsToAttack = false;
}

bool UAttackComponent::GetHasAttacked() const
{
	return _hasAttacked;
}

void UAttackComponent::Reset()
{
	_hasAttacked = false;
}

int UAttackComponent::GetAttackRange() const
{
	return _attack->GetAttackRange();
}

int UAttackComponent::GetAttackOffset() const
{
	return _attack->GetAttackOffset();
}

TArray<AGridTile*> UAttackComponent::GetAttackableTiles(AGridTile* currentTile)
{
	TArray<AGridTile*> possibleTilesToAtttack;

	if (!currentTile)
	{
		return possibleTilesToAtttack;
	}

	possibleTilesToAtttack = _attack->GetAllPossibleTilesToAttack(currentTile->GetGridIndex());

	return possibleTilesToAtttack;
}

