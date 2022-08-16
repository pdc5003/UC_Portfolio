#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_DashAction.generated.h"

/**
 * 
 */
UCLASS()
class UC_PORTFOLIO_API UCBTTaskNode_DashAction : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UCBTTaskNode_DashAction();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere)
		float LostTargetTime = 1.0f;

private:
	FVector LastTargetLocation;

	float LostTime;
};
