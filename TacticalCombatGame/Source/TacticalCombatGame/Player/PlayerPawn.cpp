// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

#include "../Grid/Grid.h"
#include "../Grid/GridTile.h"
#include "../Pathfinding/GridPathfinding.h"
#include "../UI/CharacterHUD.h"
#include "../Characters/BaseCharacter.h"

#include "Blueprint/UserWidget.h"

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

	PlayerInputComponent->BindAction("SelectObject", EInputEvent::IE_Pressed, this, &APlayerPawn::SelectObject);
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
		//set move target where character needs to move to, 
		//only if character wants to move
		//and reachable
		//do this in char self

		//now test to see if a* works
		auto path = _pathfinding->GeneratePath(_character->GetCurrentTile(), _selectedGridTile);
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("Path size is: %i"), path.Num()));
		for (int i = 0; i < path.Num(); i++)
		{
			_grid->SetTileReachable(path[i]);
		}
	}

}

AGridTile* APlayerPawn::SelectGridTile()
{
	if (!_grid)
		return nullptr;

	/*if (_character)
	{
		_character->HideReachableTiles();
		_character = nullptr;
		_characterHud->RemoveFromParent();
	}*/

	return _grid->SelectTile();
}

bool APlayerPawn::TrySelectingPlayer()
{
	FHitResult hit;
	_playerController->GetHitResultUnderCursor(_playerTraceChannel, false, hit);

	if (!hit.bBlockingHit)
		return false;

	//Get selected char
	_character = Cast<ABaseCharacter>(hit.GetActor());
	if (!_character)
		return false;

	_characterHud = Cast<UCharacterHUD>(CreateWidget(GetWorld(), _characterHudRef));
	_characterHud->SetCharacter(_character);
	_characterHud->AddToViewport();
	return true;
}

