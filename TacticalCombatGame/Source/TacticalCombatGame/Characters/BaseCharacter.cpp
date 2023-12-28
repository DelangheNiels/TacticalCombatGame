// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "HealthComponent.h"
#include "GridMovementComponent.h"

#include "Attacks/AttackComponent.h"

#include "../UI/CharacterHUD.h"

#include "Blueprint/UserWidget.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_rootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRootComponent"));
	SetRootComponent(_rootComponent);

	_mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	_mesh->SetupAttachment(_rootComponent);

	_healthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	_attackComponent = CreateDefaultSubobject<UAttackComponent>(TEXT("AttackComponent"));

	_gridMovementComponent = CreateDefaultSubobject<UGridMovementComponent>(TEXT("GridMovementComponent"));
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::OnSelected()
{
	_characterHud = Cast<UCharacterHUD>(CreateWidget(GetWorld(), _characterHudRef));
	_characterHud->SetCharacter(this);
	_characterHud->AddToViewport();
}

void ABaseCharacter::OnDeselected()
{
	_attackComponent->StopAttacking();
	_characterHud->RemoveFromParent();
	_characterHud = nullptr;
}

UHealthComponent* ABaseCharacter::GetHealthComponent() const
{
	return _healthComponent;	
}

UAttackComponent* ABaseCharacter::GetAttackComponent() const
{
	return _attackComponent;
}

UGridMovementComponent* ABaseCharacter::GetGridMovementComponent() const
{
	return _gridMovementComponent;
}

bool ABaseCharacter::GetIsControlledByPlayer() const
{
	return _isControlledByPlayer;
}

void ABaseCharacter::SetIsControlledByPlayer(bool isControlledByPlayer)
{
	_isControlledByPlayer = isControlledByPlayer;
}

void ABaseCharacter::Reset()
{
	_gridMovementComponent->Reset();
	_attackComponent->Reset();
	OnCharacterReset.Broadcast();
}

void ABaseCharacter::Lock()
{
	OnCharacterLocked.Broadcast();
}


