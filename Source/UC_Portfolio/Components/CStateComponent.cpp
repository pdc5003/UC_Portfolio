#include "CStateComponent.h"
#include "Global.h"

UCStateComponent::UCStateComponent()
{

}


void UCStateComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UCStateComponent::SetIdleMode()
{
	ChangeType(EStateType::Idle);
}

void UCStateComponent::SetRollMode()
{
	ChangeType(EStateType::Roll);
}

void UCStateComponent::SetTeleportMode()
{
	ChangeType(EStateType::Teleport);
}

void UCStateComponent::SetBackstepMode()
{
	ChangeType(EStateType::Backstep);
}

void UCStateComponent::SetActionMode()
{
	ChangeType(EStateType::Action);
}

void UCStateComponent::SetHittedMode()
{
	ChangeType(EStateType::Hitted);
}

void UCStateComponent::SetDeadMode()
{
	ChangeType(EStateType::Dead);
}

void UCStateComponent::SetStunMode()
{
	ChangeType(EStateType::Stun);
}

void UCStateComponent::SetExecutionMode()
{
	ChangeType(EStateType::Execution);
}

void UCStateComponent::SetAirHitMode()
{
	ChangeType(EStateType::AirHitted);
}

void UCStateComponent::SetDodgeMode()
{
	ChangeType(EStateType::Dodge);
}

void UCStateComponent::SetBlockMode()
{
	ChangeType(EStateType::Block);
}

void UCStateComponent::ChangeType(EStateType InNewType)
{
	EStateType prevType = Type;
	Type = InNewType;

	if (OnStateTypeChanged.IsBound())
		OnStateTypeChanged.Broadcast(prevType, InNewType);
}


