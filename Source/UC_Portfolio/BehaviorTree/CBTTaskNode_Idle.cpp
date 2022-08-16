#include "BehaviorTree/CBTTaskNode_Idle.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/PPlayer.h"
#include "Components/CBehaviorComponent.h"

UCBTTaskNode_Idle::UCBTTaskNode_Idle()
{
	NodeName = "Idle";
	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Idle::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	RemainWaitTime = FMath::FRandRange(FMath::Max(0.0f, WaitTime - RandomDeviation), (WaitTime + RandomDeviation));
	RandomRadius = FMath::FRandRange((Radius - RandomInterval), (Radius + RandomInterval));

	//Initialize
	{
		bInitialize = true;

		ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
		UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(controller);
		APPlayer* target = behavior->GetTargetPlayer();

		bInitialize &= (controller != NULL);
		bInitialize &= (behavior != NULL);
		bInitialize &= (target != NULL);

		if (bInitialize)
			controller->K2_SetFocus(target);

		
	}

	//RoamingPercentage
	{
		bRoaming = true;
		UKismetMathLibrary::RandomIntegerInRange(0, 1000) > RoamingPercentage * 10  ? bRoaming = true : bRoaming = false;
		UKismetMathLibrary::RandomIntegerInRange(0, 1000) > 500 ? Direction = 1.0f : Direction = -1.0f;
	}
	
	
	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UCBTTaskNode_Idle::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	bRoaming = false;
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	controller->StopMovement();
	return EBTNodeResult::Failed;
}

void UCBTTaskNode_Idle::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	RemainWaitTime -= DeltaSeconds;

	if (RemainWaitTime <= 0.0f)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

	if (bInitialize == true)
	{
		if (bRoaming == true)
		{
			ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
			UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(controller);
			APPlayer* target = behavior->GetTargetPlayer();
			
			if (!!target)
			{
				FVector location = target->GetActorLocation();
				FVector around = (controller->GetPawn()->GetActorLocation() - location).GetSafeNormal() * RandomRadius;

				FVector rotateLocation = around.RotateAngleAxis(AroundAngle * Direction, FVector::UpVector);
				location += rotateLocation;

				controller->MoveToLocation(location, 10.0f, false);
			}
			
		}
	}
}
