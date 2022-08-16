#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_BossAction.generated.h"

/**
 * 
 */
UCLASS()
class UC_PORTFOLIO_API UCBTTaskNode_BossAction : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UCBTTaskNode_BossAction();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	void LookRotate(UBehaviorTreeComponent& OwnerComp, float DeltaSeconds);

private:
	UPROPERTY(EditAnywhere)
		float RotateSpeed = 10.0f;
};
