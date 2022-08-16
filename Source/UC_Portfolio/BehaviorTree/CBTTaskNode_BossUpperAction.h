#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_BossUpperAction.generated.h"

UCLASS()
class UC_PORTFOLIO_API UCBTTaskNode_BossUpperAction : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UCBTTaskNode_BossUpperAction();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
