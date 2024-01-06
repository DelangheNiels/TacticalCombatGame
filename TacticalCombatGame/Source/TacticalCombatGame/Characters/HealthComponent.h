// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHealthChanged);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDied, UHealthComponent*)


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TACTICALCOMBATGAME_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	float GetCurrentHealth() const;
	float GetMaxHealth() const;
	void TakeDamage(float damage);

	FOnHealthChanged OnHealthChanged;
	FOnDied OnDied;

private:

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		float _maxhealth;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UDamageNumber> _damageNumberHudRef;

	float _currentHealth;

	//Functions

	void SpawnDamageNumber(float damage);

	void Died();
		
};
