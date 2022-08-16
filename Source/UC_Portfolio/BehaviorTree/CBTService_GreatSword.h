#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_GreatSword.generated.h"

UCLASS()
class UC_PORTFOLIO_API UCBTService_GreatSword : public UBTService
{
	GENERATED_BODY()
	
public:
	UCBTService_GreatSword();

protected:
	virtual void OnSearchStart(FBehaviorTreeSearchData& SearchData) override; 
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	void SetDashAttack();

private:
	UPROPERTY(EditAnywhere)
		int32 DashPercent = 500;

	UPROPERTY(EditAnywhere)
		int32 AttackPercent = 500;

private:
	bool bTaskEnterAble = true;
	bool bCanDashAttack = false;
};
