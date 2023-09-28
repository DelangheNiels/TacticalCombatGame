// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

#include "../Grid/Grid.h"

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

	//Make sure camera does not move in when game starts
	_desiredZoom = _springArm->TargetArmLength;
	_desiredLocation = GetActorLocation();
	_desiredYawRotation = GetActorRotation().Yaw;
	
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
	//Try selecting grid tile
	SelectGridTile();
}

void APlayerPawn::SelectGridTile()
{
	if (!_grid)
		return;

	_grid->SelectTile();
}

