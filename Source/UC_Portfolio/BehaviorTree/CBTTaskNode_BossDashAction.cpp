#include "BehaviorTree/CBTTaskNode_BossDashAction.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/PPlayer.h"
#include "Components/CStateComponent.h"
#include "Components/CBehaviorComponent.h"
#include "Components/PCombatComponent.h"
#include "Actions/PCombatAction_Melee.h"

UCBTTaskNode_BossDashAction::UCBTTaskNode_BossDashAction()
{
	NodeName = "BossDashAction";
	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_BossDashAction::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	OwnerComp.GetAIOwner()->StopMovement();
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	if (controller == NULL)
		return EBTNodeResult::Failed;

	UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(controller);
	if (behavior == NULL)
		return EBTNodeResult::Failed;

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	if (ai == NULL)
		return EBTNodeResult::Failed;

	APPlayer* target = behavior->GetTargetPlayer();
	if (target == NULL)
		return EBTNodeResult::Failed;

	controller->K2_SetFocus(target);
	controller->K2_ClearFocus();

	ai->OnDashAction();

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_BossDashAction::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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

	LookRotate(OwnerComp, DeltaSeconds);
}

void UCBTTaskNode_BossDashAction::LookRotate(UBehaviorTreeComponent& OwnerComp, float DeltaSeconds)
{
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNull(controller);

	UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(controller);
	CheckNull(behavior);

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	CheckNull(ai);

	APPlayer* target = behavior->GetTargetPlayer();
	CheckNull(target);

	FRotator ownerRotator = ai->GetActorRotation();
	FRotator targetRotator = UKismetMathLibrary::FindLookAtRotation(ai->GetActorLocation(), target->GetActorLocation());

	FRotator result = UKismetMathLibrary::RInterpTo(ownerRotator, targetRotator, DeltaSeconds, 10.0f);

	ai->SetActorRotation(result);



}