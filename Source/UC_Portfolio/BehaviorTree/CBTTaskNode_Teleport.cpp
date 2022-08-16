#include "BehaviorTree/CBTTaskNode_Teleport.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI_Boss.h"
#include "Characters/PPlayer.h"
#include "Components/CStateComponent.h"
#include "Components/CBehaviorComponent.h"

UCBTTaskNode_Teleport::UCBTTaskNode_Teleport()
{
	NodeName = "Teleport";
	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Teleport::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	OwnerComp.GetAIOwner()->StopMovement();
	TeleportTime = 0.0f;
	bTeleported = false;
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	if (controller == NULL)
		return EBTNodeResult::Failed;

	UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(controller);
	if (behavior == NULL)
		return EBTNodeResult::Failed;

	ACEnemy_AI_Boss* ai = Cast<ACEnemy_AI_Boss>(controller->GetPawn());
	if (ai == NULL)
		return EBTNodeResult::Failed;

	APPlayer* target = behavior->GetTargetPlayer();
	if (target == NULL)
		return EBTNodeResult::Failed;

	controller->K2_SetFocus(target);

	int32 InDirection = UKismetMathLibrary::RandomIntegerInRange(0, 1000);

	InDirection > 500 ? Direction = 1.0f : Direction = -1.0f;

	SetTeleportLocation(target->GetActorLocation(), controller->GetPawn()->GetActorLocation());

	//Play Teleport Montage
	{
		ai->OnTeleport();
	}

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Teleport::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	if (controller == NULL)
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

	ACEnemy_AI_Boss* ai = Cast<ACEnemy_AI_Boss>(controller->GetPawn());
	if (ai == NULL)
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

	TeleportTime += DeltaSeconds;

	if (TeleportTime >= TeleportDelay && bTeleported == false)
	{
		bTeleported = true;
		ai->SetActorLocation(TeleportLocation);
		ai->OffTeleport();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

	////Check Teleport Complete
	//{
	//	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);
	//
	//	if (state->IsIdleMode())
	//		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	//}
}

void UCBTTaskNode_Teleport::SetTeleportLocation(const FVector& InTargetLocation, const FVector& InOwnerLocation)
{
	TeleportLocation = InTargetLocation;
	FVector around = (InOwnerLocation - InTargetLocation).GetSafeNormal() * DistanceToTarget;

	FVector rotateLocation = around.RotateAngleAxis(Angle * Direction, FVector::UpVector);
	TeleportLocation += rotateLocation;
}