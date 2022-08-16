#include "BehaviorTree/CBTTaskNode_BossIdle.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/PPlayer.h"
#include "Components/CBehaviorComponent.h"

UCBTTaskNode_BossIdle::UCBTTaskNode_BossIdle()
{
	NodeName = "BossIdle";
	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_BossIdle::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	OwnerComp.GetAIOwner()->StopMovement();

	RemainWaitTime = FMath::FRandRange(FMath::Max(0.0f, WaitTime - RandomDeviation), (WaitTime + RandomDeviation));

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UCBTTaskNode_BossIdle::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	controller->StopMovement();
	return EBTNodeResult::Failed;
}

void UCBTTaskNode_BossIdle::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	RemainWaitTime -= DeltaSeconds;

	if (RemainWaitTime <= 0.0f)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
