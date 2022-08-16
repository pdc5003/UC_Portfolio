#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Teleport.generated.h"

/**
 * 
 */
UCLASS()
class UC_PORTFOLIO_API UCBTTaskNode_Teleport : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UCBTTaskNode_Teleport();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	void SetTeleportLocation(const FVector& InTargetLocation, const FVector& InOwnerLocation);

private:
	FVector TeleportLocation;

private:
	UPROPERTY(EditAnywhere)
		float DistanceToTarget = 500.0f;

	UPROPERTY(EditAnywhere)
		float Angle = 50.0f;
	
	UPROPERTY(EditAnywhere)
		float TeleportDelay = 0.8f;

	float TeleportTime = 0.0f;
	float Direction = 1.0f;

	bool bTeleported = false;
};
