#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Trace.generated.h"

/**
 * 
 */
UCLASS()
class UC_PORTFOLIO_API UCBTTaskNode_Trace : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UCBTTaskNode_Trace();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	UPROPERTY(EditAnywhere, meta = (ClampMin = 5.00, UIMin = 5.00))
		float AcceptanceRadius = 5.0f;
private:
	FVector TargetLocation;
};
