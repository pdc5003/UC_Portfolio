#include "BehaviorTree/CBTTaskNode_Avoid.h"
#include "Global.h"
#include "Components/CStateComponent.h"
#include "Characters/CAIController.h"

UCBTTaskNode_Avoid::UCBTTaskNode_Avoid()
{
	NodeName = "Avoid";
	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Avoid::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	if (controller == NULL)
		return EBTNodeResult::Failed;

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(controller->GetPawn());
	if(state == NULL)
		return EBTNodeResult::Failed;

	state->SetDodgeMode();

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Avoid::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	if (controller == NULL)
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(controller->GetPawn());
	if(state == NULL)
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

	if(state->IsIdleMode()==true)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

}
