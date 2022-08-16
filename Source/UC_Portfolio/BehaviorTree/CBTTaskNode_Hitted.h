#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Hitted.generated.h"

UCLASS()
class UC_PORTFOLIO_API UCBTTaskNode_Hitted : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UCBTTaskNode_Hitted();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
