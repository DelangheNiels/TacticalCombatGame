// Fill out your copyright notice in the Description page of Project Settings.


#include "StateMachineComponent.h"
#include "State.h"

// Sets default values for this component's properties
UStateMachineComponent::UStateMachineComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UStateMachineComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void UStateMachineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateCurrentState(DeltaTime);
	CheckStateTransitions();
}

void UStateMachineComponent::SetInitialState(UState* state)
{
	if (state)
		_currentState = state;
}

void UStateMachineComponent::AddStateTransition(UState* fromState, UState* toState, TFunction<bool()> transitionFunction)
{
	FStateTransition stateTransition;
	stateTransition.fromState = fromState;
	stateTransition.toState = toState;
	stateTransition.transitionFunction = transitionFunction;

	if (stateTransition.fromState == nullptr || stateTransition.toState == nullptr)
		return;

	_stateTransitions.Add(stateTransition);
}

void UStateMachineComponent::UpdateCurrentState(float deltaTime)
{
	if (!_currentState)
		return;

	_currentState->Update(deltaTime);
}

void UStateMachineComponent::CheckStateTransitions()
{
	if (_stateTransitions.Num() <= 1)
		return;

	for (int i = 0; i < _stateTransitions.Num(); i++)
	{
		FStateTransition stateTransition = _stateTransitions[i];

		if (stateTransition.fromState == nullptr || stateTransition.toState == nullptr)
			continue;


		if (stateTransition.fromState == _currentState && stateTransition.transitionFunction() == true)
		{
			_currentState->OnExit();
			_currentState = stateTransition.toState;
			_currentState->OnEnter();
			return;
		}

	}
}

