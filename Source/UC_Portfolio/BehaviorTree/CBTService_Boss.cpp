#include "BehaviorTree/CBTService_Boss.h"
#include "Global.h"
#include "Characters/CAIController_Boss.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/PPlayer.h"
#include "Components/CBehaviorComponent.h"
#include "Components/CStateComponent.h"

UCBTService_Boss::UCBTService_Boss()
{
	NodeName = "Boss";
}

void UCBTService_Boss::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{
	Super::OnSearchStart(SearchData);
	bTaskEnterAble = true;
}

void UCBTService_Boss::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ACAIController_Boss* controller = Cast<ACAIController_Boss>(OwnerComp.GetOwner());
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

	if (bTaskEnterAble)
	{
		bTaskEnterAble = false;
		controller->ExecutePattern();
	}

}

