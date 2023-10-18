// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

#include "../UI/DamageNumber.h"

#include "Blueprint/UserWidget.h"

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

float UHealthComponent::GetMaxHealth() const
{
	return _maxhealth;
}

void UHealthComponent::TakeDamage(float damage)
{
	_currentHealth -= damage;

	SpawnDamageNumber(damage);

	OnHealthChanged.Broadcast();

	if (_currentHealth > 0)
		return;

	OnDied.Broadcast(this);
	GetOwner()->Destroy();
}

void UHealthComponent::SpawnDamageNumber(float damage)
{
	auto damageNumber = Cast<UDamageNumber>(CreateWidget(GetWorld(), _damageNumberHudRef));
	damageNumber->Instantiate(damage, GetOwner()->GetActorLocation());
	damageNumber->AddToViewport();
}

