// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyPlayer.h"

#include "../Characters/BaseCharacter.h"
#include "../Characters/HealthComponent.h"

#include "../TacticalCombatGameGameModeBase.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyPlayer::AEnemyPlayer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyPlayer::BeginPlay()
{
	Super::BeginPlay();

	GetAllCharactersToControl();

	_gamemode = Cast<ATacticalCombatGameGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	if (!_gamemode)
		return;

	//let game mode know that enemy player is spawned in
	_gamemode->CreateEnemyPlayerStates(this);
	
}

// Called every frame
void AEnemyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyPlayer::ControlCharacters()
{
	GetWorld()->GetTimerManager().SetTimer(_controlCharactersTimer, this, &AEnemyPlayer::ControlNextCharacter, 2.0f, true);
}

void AEnemyPlayer::ResetControlledCharacters()
{
	_charactersToControl = _controlledCharacters;
	_controlledCharacters.Empty();

	for (int i = 0; i < _charactersToControl.Num(); i++)
	{
		_charactersToControl[i]->Reset();
	}
}

void AEnemyPlayer::GetAllCharactersToControl()
{
	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseCharacter::StaticClass(), foundActors);

	if (foundActors.Num() <= 0)
		return;

	for (int i = 0; i < foundActors.Num(); i++)
	{
		auto character = Cast<ABaseCharacter>(foundActors[i]);

		if (!character->GetIsControlledByPlayer())
		{
			_charactersToControl.Add(character);
			character->GetHealthComponent()->OnDied.AddUObject(this, &AEnemyPlayer::OnControlledCharacterDie);
		}
	}
}

void AEnemyPlayer::OnControlledCharacterDie(UHealthComponent* healthcomp)
{
	healthcomp->OnDied.RemoveAll(this);

	auto character = Cast<ABaseCharacter>(healthcomp->GetOwner());

	if (!character)
		return;

	_charactersToControl.Remove(character);
}

void AEnemyPlayer::ControlNextCharacter()
{
	if (_charactersToControl.Num() <= 0)
	{
		if (_gamemode)
			_gamemode->ChangeTurn();
		
		GetWorld()->GetTimerManager().ClearTimer(_controlCharactersTimer);
		return;
	}

	ABaseCharacter* character = _charactersToControl[0];

	//move
	//see if attack
	//see if can move back

	_charactersToControl.Remove(character);
	_controlledCharacters.Add(character);

	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("Controlling character"));
}

