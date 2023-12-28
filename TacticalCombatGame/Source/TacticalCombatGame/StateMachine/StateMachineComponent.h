// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StateMachineComponent.generated.h"

class UState;

USTRUCT(BlueprintType)
struct FStateTransition
{
	GENERATED_BODY()

	UPROPERTY()
		UState* fromState;
	UPROPERTY()
		UState* toState;

	TFunction<bool()> transitionFunction;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TACTICALCOMBATGAME_API UStateMachineComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStateMachineComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
		void SetInitialState(UState* state);
	void AddStateTransition(UState* fromState, UState* toState, TFunction<bool()> transitionFunction);

	UFUNCTION()
		void UpdateCurrentState(float deltaTime);

private:

	UPROPERTY()
		UState* _currentState;

	UPROPERTY()
		TArray<FStateTransition> _stateTransitions;

	//Functions
	void CheckStateTransitions();


};
