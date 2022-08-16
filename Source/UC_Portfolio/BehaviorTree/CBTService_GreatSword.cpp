#include "BehaviorTree/CBTService_GreatSword.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/PPlayer.h"
#include "Components/CBehaviorComponent.h"
#include "Components/CStateComponent.h"

UCBTService_GreatSword::UCBTService_GreatSword()
{
	NodeName = "GreatSwrod";
}

void UCBTService_GreatSword::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{
	Super::OnSearchStart(SearchData);
	bTaskEnterAble = true;
	SetDashAttack();
}

void UCBTService_GreatSword::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNull(controller);

	UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(controller);
	CheckNull(behavior);

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	CheckNull(ai);

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);
	CheckNull(state);

	APPlayer* target = behavior->GetTargetPlayer();
	
	//Dead Or Execution;
	{
		if (state->IsDeadMode() || state->IsExecutionMode())
		{
			controller->StopMovement();
			controller->K2_ClearFocus();
			behavior->SetDeadMode();
			return;
		}
	}


	if (target == NULL)
	{
		behavior->SetIdleMode();
		return;
	}

	

	//Hitted or Stun or Execution
	{
		bool b = false;
		b |= state->IsHittedMode();
		b |= state->IsAirHittedMode();
		b |= state->IsStunMode();
		if (b == true)
		{
			behavior->SetHittedMode();
			return;
		}
	}

	if (!!target)
	{
		float distance = ai->GetDistanceTo(target);

		bool bDashAttackAble = true;
		bDashAttackAble &= distance > controller->GetActionRange();
		bDashAttackAble &= distance < controller->GetMiddleRange();

		if (bDashAttackAble == true && bCanDashAttack == true)
		{
			bTaskEnterAble = false;
			behavior->SetDashActionMode();
			return;
		}

		//Trace
		if (distance > controller->GetActionRange())
		{
			behavior->SetTraceMode();
			bTaskEnterAble = true;
			return;
		}

		if (bTaskEnterAble == true)
		{
			bTaskEnterAble = false;
			//Action
			if (distance < controller->GetActionRange())
			{
				int32 aggresive = UKismetMathLibrary::RandomIntegerInRange(0, 1000);
				aggresive > AttackPercent ? behavior->SetActionMode() : behavior->SetIdleMode();

				return;

			}
		}
		
	}

}

void UCBTService_GreatSword::SetDashAttack()
{
	int32 beginDashAtk = UKismetMathLibrary::RandomIntegerInRange(0, 1000);
	beginDashAtk > DashPercent ? bCanDashAttack = true : bCanDashAttack = false;
}
