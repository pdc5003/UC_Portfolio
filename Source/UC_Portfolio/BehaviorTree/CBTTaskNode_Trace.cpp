#include "BehaviorTree/CBTTaskNode_Trace.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/PPlayer.h"
#include "Components/CBehaviorComponent.h"

UCBTTaskNode_Trace::UCBTTaskNode_Trace()
{
	NodeName = "Trace";
	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Trace::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(controller);
	APPlayer* target = behavior->GetTargetPlayer();

	if(target==NULL)
		return EBTNodeResult::Failed;

	TargetLocation = target->GetActorLocation();

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	
	if (!!ai)
	{
		if(ai->GetDistanceTo(target)<=AcceptanceRadius)
			return  EBTNodeResult::Succeeded;
	}



	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Trace::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());

	EPathFollowingRequestResult::Type type = controller->MoveToLocation(TargetLocation, AcceptanceRadius, false);

	if(type== EPathFollowingRequestResult::AlreadyAtGoal)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

}
