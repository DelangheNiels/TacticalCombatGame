// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

#include "../Grid/Grid.h"
#include "../Grid/GridTile.h"

#include "../Pathfinding/GridPathfinding.h"

#include "../Characters/BaseCharacter.h"
#include "../Characters/GridMovementComponent.h"
#include "../Characters/HealthComponent.h"

#include "../TacticalCombatGameGameModeBase.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_rootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRootComponent"));
	SetRootComponent(_rootComponent);

	_springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	_springArm->SetupAttachment(_rootComponent);

	_camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	_camera->SetupAttachment(_springArm);

}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	_grid = Cast<AGrid>(UGameplayStatics::GetActorOfClass(GetWorld(), AGrid::StaticClass()));
	_pathfinding = Cast<AGridPathfinding>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridPathfinding::StaticClass()));
	//Make sure camera does not move in when game starts
	_desiredZoom = _springArm->TargetArmLength;
	_desiredLocation = GetActorLocation();
	_desiredYawRotation = GetActorRotation().Yaw;

	_playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	GetAllCharactersToControl();

	_gamemode = Cast<ATacticalCombatGameGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	if (!_gamemode)
		return;

	//let game mode know that player is spawned in
	_gamemode->CreatePlayerStates(this);
	
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SmoothCameraMovement(DeltaTime);
}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("CameraZoom", this, &APlayerPawn::ZoomCamera);
	PlayerInputComponent->BindAxis("ForwardCameraMovement", this ,&APlayerPawn::ForwardCameraMovement);
	PlayerInputComponent->BindAxis("RightCameraMovement", this, &APlayerPawn::RightCameraMovement);
	PlayerInputComponent->BindAxis("CameraRotation", this, &APlayerPawn::RotateCamera);
	
	PlayerInputComponent->BindAction("RotateCharacterLeft", EInputEvent::IE_Pressed,this, &APlayerPawn::RotateSelectedCharacterLeft);
	PlayerInputComponent->BindAction("RotateCharacterRight", EInputEvent::IE_Pressed, this, &APlayerPawn::RotateSelectedCharacterRight);
	PlayerInputComponent->BindAction("SelectObject", EInputEvent::IE_Pressed, this, &APlayerPawn::SelectObject);
	PlayerInputComponent->BindAction("DeselectObject", EInputEvent::IE_Pressed, this, &APlayerPawn::DeselectObject);
}

void APlayerPawn::ResetControlledCharacters()
{
	for (int i = 0; i < _charactersToControl.Num(); i++)
	{
		_charactersToControl[i]->Reset();
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("Resetting char"));
	}
}

void APlayerPawn::LockControlledCharacters()
{
	for (int i = 0; i < _charactersToControl.Num(); i++)
	{
		_charactersToControl[i]->Lock();
	}
}

void APlayerPawn::ZoomCamera(float axisValue)
{
	float armLength = _desiredZoom + axisValue * _zoomSpeed;
	_desiredZoom = FMath::Clamp(armLength, _minimumArmLength, _maximumArmLength);
}

void APlayerPawn::ForwardCameraMovement(float axisValue)
{
	_desiredLocation += axisValue * _movementSpeed * GetActorForwardVector();
}

void APlayerPawn::RightCameraMovement(float axisValue)
{
	_desiredLocation += axisValue * _movementSpeed * GetActorRightVector();
}

void APlayerPawn::RotateCamera(float axisValue)
{
	if (axisValue == 0)
		return;

	_isRotating = true;
	_desiredYawRotation += axisValue * _rotationSpeed;
}

void APlayerPawn::SmoothCameraMovement(float deltaTime)
{
	//Camera zoom
	_springArm->TargetArmLength = FMath::FInterpTo(_springArm->TargetArmLength, _desiredZoom, deltaTime, _smoothInterpolateSpeed);

	//Camera movement
	FVector newLocation = FMath::VInterpTo(GetActorLocation(), _desiredLocation, deltaTime, _smoothInterpolateSpeed);
	SetActorLocation(newLocation);

	//Camera rotation
	if (!_isRotating)
		return;

	float newYawRotation = _desiredYawRotation * deltaTime;
	FRotator newRotation = FRotator(GetActorRotation().Pitch, newYawRotation, GetActorRotation().Roll);
	SetActorRotation(newRotation);
	_isRotating = false;

}

void APlayerPawn::SelectObject()
{
	if (TrySelectingPlayer())
		return;

	//Try selecting grid tile
	_selectedGridTile = SelectGridTile();

	if (_character && _selectedGridTile)
	{
		_character->GetGridMovementComponent()->CreatePathToDestination(*_selectedGridTile);
	}

}

void APlayerPawn::DeselectObject()
{
	if (!_character)
		return;

	_character->OnDeselected();
	_character = nullptr;
}

void APlayerPawn::RotateSelectedCharacterLeft()
{
	if (!_character)
		return;

	FRotator characterRotation = _character->GetActorRotation();
	characterRotation.Yaw -= 90;

	_character->SetActorRotation(characterRotation);
	_character->OnCharacterRotated.Broadcast();
}

void APlayerPawn::RotateSelectedCharacterRight()
{
	if (!_character)
		return;

	FRotator characterRotation = _character->GetActorRotation();
	characterRotation.Yaw += 90;

	_character->SetActorRotation(characterRotation);
	_character->OnCharacterRotated.Broadcast();
}


AGridTile* APlayerPawn::SelectGridTile()
{
	if (!_grid)
		return nullptr;

	return _grid->SelectTile();
}

bool APlayerPawn::TrySelectingPlayer()
{
	FHitResult hit;
	_playerController->GetHitResultUnderCursor(_playerTraceChannel, false, hit);

	if (!hit.bBlockingHit)
		return false;

	//Deselect the previously selected character
	if (_character)
		_character->OnDeselected();

	//Get selected char
	_character = Cast<ABaseCharacter>(hit.GetActor());
	if (!_character)
		return false;

	_character->OnSelected();
	
	return true;
}

void APlayerPawn::GetAllCharactersToControl()
{
	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseCharacter::StaticClass(), foundActors);

	if (foundActors.Num() <= 0)
		return;

	for (int i = 0; i < foundActors.Num(); i++)
	{
		auto character = Cast<ABaseCharacter>(foundActors[i]);

		if (character->GetIsControlledByPlayer())
			_charactersToControl.Add(character);
	}
}

void APlayerPawn::OnControlledCharacterDie(UHealthComponent* healthcomp)
{
	healthcomp->OnDied.RemoveAll(this);

	auto character = Cast<ABaseCharacter>(healthcomp->GetOwner());

	if (!character)
		return;

	_charactersToControl.Remove(character);

}

