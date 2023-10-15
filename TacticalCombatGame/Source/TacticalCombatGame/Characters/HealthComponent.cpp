// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	_currentHealth = _maxhealth;
	
}

float UHealthComponent::GetCurrentHealth() const
{
	return _currentHealth;
}

void UHealthComponent::TakeDamage(float damage)
{
	_currentHealth -= damage;

	if (_currentHealth <= 0)
		GetOwner()->Destroy();
}

