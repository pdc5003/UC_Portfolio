#include "BehaviorTree/CBTTaskNode_BossStrafe.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI_Boss.h"
#include "Characters/PPlayer.h"
#include "Components/CBehaviorComponent.h"

UCBTTaskNode_BossStrafe::UCBTTaskNode_BossStrafe()
{
	NodeName = "BossStrafe";
	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_BossStrafe::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	OwnerComp.GetAIOwner()->StopMovement();
	RemainWaitTime = FMath::FRandRange(FMath::Max(0.0f, WaitTime - RandomDeviation), (WaitTime + RandomDeviation));
	RandomRadius = FMath::FRandRange((Radius - RandomInterval), (Radius + RandomInterval));

	SummonStartTime = UKismetSystemLibrary::GetGameTimeInSeconds(GetWorld());
	SummonStartTime += SummonDelay*0.5f;

	bInitialize = true;

	UKismetMathLibrary::RandomIntegerInRange(0, 1000) > 500 ? Direction = 1.0f : Direction = -1.0f;

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(controller);
	APPlayer* target = behavior->GetTargetPlayer();

	bInitialize &= (controller != NULL);
	bInitialize &= (behavior != NULL);
	bInitialize &= (target != NULL);

	if (bInitialize)
		controller->K2_SetFocus(target);

	return EBTNodeResult::InProgress;
}


void UCBTTaskNode_BossStrafe::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	RemainWaitTime -= DeltaSeconds;
	
	if (RemainWaitTime <= 0.0f)
	{
		ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
		controller->StopMovement();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

	if (bInitialize == true)
	{
		ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
		UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(controller);
		APPlayer* target = behavior->GetTargetPlayer();

		if (!!target)
		{
			SummonSword(OwnerComp);

			FVector location = target->GetActorLocation();
			FVector around = (controller->GetPawn()->GetActorLocation() - location).GetSafeNormal() * RandomRadius;

			FVector rotateLocation = around.RotateAngleAxis(AroundAngle * Direction, FVector::UpVector);
			location += rotateLocation;

			controller->MoveToLocation(location, 10.0f, false);
		}
	}
}

void UCBTTaskNode_BossStrafe::SummonSword(UBehaviorTreeComponent& OwnerComp)
{
	if (UKismetSystemLibrary::GetGameTimeInSeconds(GetWorld()) - SummonStartTime > SummonDelay)
	{
		SummonStartTime = UKismetSystemLibrary::GetGameTimeInSeconds(GetWorld());
		ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
		CheckNull(controller);

		ACEnemy_AI_Boss* boss = Cast<ACEnemy_AI_Boss>(controller->GetPawn());
		CheckNull(boss);

		boss->SpawnSummonSword();
	}
}
