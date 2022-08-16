#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_BossIdle.generated.h"

UCLASS()
class UC_PORTFOLIO_API UCBTTaskNode_BossIdle : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UCBTTaskNode_BossIdle();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	UPROPERTY(EditAnywhere, Category = "StandBy", meta = (ClampMin = 0.00, UIMin = 0.00))
		float WaitTime;

	UPROPERTY(EditAnywhere, Category = "StandBy", meta = (ClampMin = 0.00, UIMin = 0.00))
		float RandomDeviation;

private:
	float RemainWaitTime;
};
