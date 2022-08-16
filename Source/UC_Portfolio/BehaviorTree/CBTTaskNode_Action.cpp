#include "BehaviorTree/CBTTaskNode_Action.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/PPlayer.h"
#include "Components/CStateComponent.h"
#include "Components/CBehaviorComponent.h"
#include "Components/PCombatComponent.h"
#include "Actions/PCombatAction_Melee.h"

UCBTTaskNode_Action::UCBTTaskNode_Action()
{
	NodeName = "Action";
	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Action::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetAIOwner());
	UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(OwnerComp.GetAIOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	APPlayer* target = behavior->GetTargetPlayer();

	bool b = true; 
	b &= (controller != nullptr);
	b &= (behavior != nullptr);
	b &= (ai != nullptr);
	b &= (target != nullptr);


	if (b == false)
		return EBTNodeResult::Failed;

	controller->ClearFocus(EAIFocusPriority::Gameplay);
	NumberOfCombo = UKismetMathLibrary::RandomIntegerInRange(1, MaxComboCount);
	CurrentComboCount = 0;

	++CurrentComboCount;
	ai->OnAction();

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Action::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	if (controller == NULL)
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	if (ai == NULL)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

	//Check EndAction
	{
		UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);

		if (state == nullptr)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
		}

		if (state->IsIdleMode())
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

	//Check ComboExist
	if (CurrentComboCount < NumberOfCombo)
	{
		UPCombatComponent* combat = CHelpers::GetComponent<UPCombatComponent>(ai);
		if (combat == nullptr)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
		}

		APCombatAction_Melee* combatMelee = Cast<APCombatAction_Melee>(combat->GetCurrent()->GetCombatAction());
		if (combatMelee == nullptr)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
		}

		if (combatMelee->IsEnableCombo())
		{
			CurrentComboCount++;
			ai->OnAction();
		}
	}
}
