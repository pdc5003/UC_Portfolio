#include "BehaviorTree/CBTTaskNode_BossUpperAction.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/PPlayer.h"
#include "Components/CStateComponent.h"
#include "Components/CBehaviorComponent.h"
#include "Components/PCombatComponent.h"
#include "Actions/PCombatAction_Melee.h"

UCBTTaskNode_BossUpperAction::UCBTTaskNode_BossUpperAction()
{
	NodeName = "BossUpperAction";
	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_BossUpperAction::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	ai->OnUpperAction();

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_BossUpperAction::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	if (controller == NULL)
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	if (ai == NULL)
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

	//Check ComboExist
	{
		UPCombatComponent* combat = CHelpers::GetComponent<UPCombatComponent>(ai);
		if (combat == NULL)
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

		APCombatAction_Melee* combatMelee = Cast<APCombatAction_Melee>(combat->GetCurrent()->GetCombatAction());
		if (combatMelee == NULL)
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);


		if (combatMelee->IsEnableCombo())
			ai->OnAction();

	}

	//Check EndAction
	{
		UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);

		if (state->IsIdleMode())
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}