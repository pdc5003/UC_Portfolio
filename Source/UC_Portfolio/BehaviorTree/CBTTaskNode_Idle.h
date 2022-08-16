#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Idle.generated.h"

UCLASS()
class UC_PORTFOLIO_API UCBTTaskNode_Idle : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UCBTTaskNode_Idle();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	UPROPERTY(EditAnywhere, Category = "StandBy", meta = (ClampMin = 0.00, UIMin = 0.00))
		float WaitTime;

	UPROPERTY(EditAnywhere, Category = "StandBy", meta = (ClampMin = 0.00, UIMin = 0.00))
		float RandomDeviation;


public:
	UPROPERTY(EditAnywhere, Category = "Roaming")
		int32 RoamingPercentage = 25;

	UPROPERTY(EditAnywhere, Category = "Roaming")
		float Radius;

	UPROPERTY(EditAnywhere, Category = "Roaming")
		float RandomInterval;

	UPROPERTY(EditAnywhere, Category = "Roaming")
		float AroundAngle;

	float RandomRadius;
	float Direction = 1.0f;

private:
	bool bInitialize = true;
	bool bRoaming = true;

	float RemainWaitTime;
	
};
