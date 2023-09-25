// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

UCLASS()
class TACTICALCOMBATGAME_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	UPROPERTY(EditInstanceOnly, meta = (AllowPrivateAccess = "true"))
		USceneComponent* _rootComponent;

	UPROPERTY(EditInstanceOnly, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* _springArm;

	UPROPERTY(EditInstanceOnly, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* _camera;

	UPROPERTY(EditAnywhere, category = "Camera Zoom", meta = (AllowPrivateAccess = "true"))
		float _minimumArmLength = 200;
	UPROPERTY(EditAnywhere, category = "Camera Zoom", meta = (AllowPrivateAccess = "true"))
		float _maximumArmLength = 5000;
	UPROPERTY(EditAnywhere, category = "Camera Zoom", meta = (AllowPrivateAccess = "true"))
		float _zoomSpeed = 40;
	UPROPERTY(EditAnywhere, category = "Camera Zoom", meta = (AllowPrivateAccess = "true"))
		float _smoothInterpolateSpeed = 2;
	float _desiredZoom;

	UPROPERTY(EditAnywhere, category = "Camera Movement", meta = (AllowPrivateAccess = "true"))
		float _movementSpeed = 40;
	FVector _desiredLocation;

	UPROPERTY(EditAnywhere, category = "Camera Rotation", meta = (AllowPrivateAccess = "true"))
		float _rotationSpeed = 60.0f;
	float _desiredYawRotation;
	bool _isRotating;

	//Functions

	void ZoomCamera(float axisValue);

	void ForwardCameraMovement(float axisValue);
	void RightCameraMovement(float axisValue);

	void RotateCamera(float axisValue);

	void SmoothCameraMovement(float deltaTime);

};
