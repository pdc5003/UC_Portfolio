

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_BossStrafe.generated.h"

/**
 * 
 */
UCLASS()
class UC_PORTFOLIO_API UCBTTaskNode_BossStrafe : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UCBTTaskNode_BossStrafe();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	//virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	UPROPERTY(EditAnywhere, Category = "StrafeTime", meta = (ClampMin = 0.00, UIMin = 0.00))
		float WaitTime;

	UPROPERTY(EditAnywhere, Category = "StrafeTime", meta = (ClampMin = 0.00, UIMin = 0.00))
		float RandomDeviation;

private:
	void SummonSword(UBehaviorTreeComponent& OwnerComp);


public:
	UPROPERTY(EditAnywhere, Category = "StrafeFactor")
		float Radius;

	UPROPERTY(EditAnywhere, Category = "StrafeFactor")
		float RandomInterval;

	UPROPERTY(EditAnywhere, Category = "StrafeFactor")
		float AroundAngle;

	float RandomRadius;
	float Direction = 1.0f;

private:
	bool bInitialize = true;

	float RemainWaitTime;

private:
	UPROPERTY(EditAnywhere)
		float SummonDelay = 0.35f;

	float SummonStartTime;
};
