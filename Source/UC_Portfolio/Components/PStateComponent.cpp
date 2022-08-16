#include "Components/PStateComponent.h"
#include "Global.h"
UPStateComponent::UPStateComponent()
{
}


void UPStateComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UPStateComponent::SetIdleMode()
{
	ChangeType(EState::Idle);
}

void UPStateComponent::SetRollMode()
{
	ChangeType(EState::Roll);
}

void UPStateComponent::SetDodgeMode()
{
	ChangeType(EState::Dodge);
}

void UPStateComponent::SetActionMode()
{
	ChangeType(EState::Action);
}

void UPStateComponent::SetHittedMode()
{
	ChangeType(EState::Hitted);
}

void UPStateComponent::SetDeadMode()
{
	ChangeType(EState::Dead);
}

void UPStateComponent::ChangeType(EState InNewType)
{
	EState prevType = Type;
	Type = InNewType;

	/*if (OnStateTypeChanged.IsBound())
		OnStateTypeChanged.Broadcast(prevType, InNewType);*/
}