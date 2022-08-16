#include "CBehaviorComponent.h"
#include "Global.h"
#include "Characters/PPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"

UCBehaviorComponent::UCBehaviorComponent()
{

}

bool UCBehaviorComponent::IsIdleMode()
{
	return GetType() == EBehaviorType::Idle;
}

bool UCBehaviorComponent::IsTraceMode()
{
	return GetType() == EBehaviorType::Trace;
}

bool UCBehaviorComponent::IsActionMode()
{
	return GetType() == EBehaviorType::Action;
}

bool UCBehaviorComponent::IsHittedMode()
{
	return GetType() == EBehaviorType::Hitted;
}

bool UCBehaviorComponent::IsAvoidMode()
{
	return GetType() == EBehaviorType::Avoid;
}

bool UCBehaviorComponent::IsDeadMode()
{
	return GetType() == EBehaviorType::Dead;
}

bool UCBehaviorComponent::IsRoamingMode()
{
	return GetType() == EBehaviorType::Roaming;
}

bool UCBehaviorComponent::IsTeleportMode()
{
	return GetType() == EBehaviorType::Teleport;
}

bool UCBehaviorComponent::IsDashActionMode()
{
	return GetType() == EBehaviorType::DashAction;
}

bool UCBehaviorComponent::IsUpperActionMode()
{
	return GetType() == EBehaviorType::UpperAction;
}

void UCBehaviorComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCBehaviorComponent::SetIdleMode()
{
	ChangeType(EBehaviorType::Idle);
}

void UCBehaviorComponent::SetTraceMode()
{
	ChangeType(EBehaviorType::Trace);
}

void UCBehaviorComponent::SetActionMode()
{
	ChangeType(EBehaviorType::Action);
}

void UCBehaviorComponent::SetDashActionMode()
{
	ChangeType(EBehaviorType::DashAction);
}

void UCBehaviorComponent::SetUpperActionMode()
{
	ChangeType(EBehaviorType::UpperAction);
}

void UCBehaviorComponent::SetHittedMode()
{
	ChangeType(EBehaviorType::Hitted);
}

void UCBehaviorComponent::SetAvoidMode()
{
	ChangeType(EBehaviorType::Avoid);
}

void UCBehaviorComponent::SetDeadMode()
{
	ChangeType(EBehaviorType::Dead);
}

void UCBehaviorComponent::SetRoamingMode()
{
	ChangeType(EBehaviorType::Roaming);
}

void UCBehaviorComponent::SetTeleportMode()
{
	ChangeType(EBehaviorType::Teleport);
}


APPlayer* UCBehaviorComponent::GetTargetPlayer()
{
	return Cast<APPlayer>(Blackboard->GetValueAsObject(PlayerKey));
}

void UCBehaviorComponent::ChangeType(EBehaviorType InType)
{
	EBehaviorType type = GetType();
	Blackboard->SetValueAsEnum(BehaviorKey, (uint8)InType);

	if (OnBehaviorTypeChanged.IsBound())
		OnBehaviorTypeChanged.Broadcast(type, InType);
}

EBehaviorType UCBehaviorComponent::GetType()
{
	return (EBehaviorType)Blackboard->GetValueAsEnum(BehaviorKey);
}