// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseCharacter.generated.h"

class AGridTile;
class AGrid;
class AGridPathfinding;
class UHealthComponent;
class UAttackComponent;
class UGridMovementComponent;

DECLARE_MULTICAST_DELEGATE(FOnCharacterReset);
DECLARE_MULTICAST_DELEGATE(FOnCharacterLock);
DECLARE_MULTICAST_DELEGATE(FOnCharacterRotated);

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

	void OnSelected();
	void OnDeselected();

	UHealthComponent* GetHealthComponent() const;
	UAttackComponent* GetAttackComponent() const;
	UGridMovementComponent* GetGridMovementComponent() const;

	bool GetIsControlledByPlayer() const;
	void SetIsControlledByPlayer(bool isControlledByPlayer);

	void Reset();
	void Lock();

	FOnCharacterLock OnCharacterLocked;
	FOnCharacterReset OnCharacterReset;
	FOnCharacterRotated OnCharacterRotated;

private:

	UPROPERTY()
		USceneComponent* _rootComponent;

	UPROPERTY(EditInstanceOnly, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* _mesh;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCharacterHUD> _characterHudRef;
	UPROPERTY()
		UCharacterHUD* _characterHud;

	UPROPERTY(EditInstanceOnly)
		UHealthComponent* _healthComponent;

	UPROPERTY(EditInstanceOnly)
		UAttackComponent* _attackComponent;

	UPROPERTY(EditInstanceOnly)
		UGridMovementComponent* _gridMovementComponent;

	bool _isControlledByPlayer;

	void OnDied(UHealthComponent* healthcomp);


};
