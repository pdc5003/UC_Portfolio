#include "BehaviorTree/CBTTaskNode_DashAction.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/PPlayer.h"
#include "Components/CStateComponent.h"
#include "Components/CBehaviorComponent.h"
#include "Components/PCombatComponent.h"
#include "Actions/PCombatAction_Melee.h"

UCBTTaskNode_DashAction::UCBTTaskNode_DashAction()
{
	NodeName = "DashAction";
	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_DashAction::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	LostTime = LostTargetTime;
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	if (IsValid(controller)==false)
		return EBTNodeResult::Failed;

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	if (IsValid(ai)==false)
		return EBTNodeResult::Failed;

	UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(controller);
	if (IsValid(behavior)==false)
		return EBTNodeResult::Failed;

	controller->StopMovement();
	controller->K2_ClearFocus();
	ai->OnDashAction();


	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_DashAction::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	if (controller == NULL)
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	if (ai == NULL)
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

	//Check EndAction
	{
		UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);

		if (state->IsIdleMode())
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
		}
	}

	UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(controller);
	if (behavior == NULL)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	APPlayer* target = behavior->GetTargetPlayer();
	if (target == NULL)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	LostTime -= DeltaSeconds;

	if (LostTime <= 0.0f)
	{
		FRotator rotate = UKismetMathLibrary::FindLookAtRotation(ai->GetActorLocation(), LastTargetLocation);
		rotate.Pitch = 0;
		rotate.Roll = 0;
		ai->SetActorRotation(rotate);
		return;
	}

	//Rotate For Target
	{
		LastTargetLocation = target->GetActorLocation();
		FRotator rotate = UKismetMathLibrary::FindLookAtRotation(ai->GetActorLocation(), target->GetActorLocation());
		rotate.Pitch = 0;
		rotate.Roll = 0;
		ai->SetActorRotation(rotate);
	}



}
