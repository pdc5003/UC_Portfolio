#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Action.generated.h"

UCLASS()
class UC_PORTFOLIO_API UCBTTaskNode_Action : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UCBTTaskNode_Action();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere)
		int32 MaxComboCount = 4;

	int32 NumberOfCombo = 0;
	int32 CurrentComboCount = 0;
};
