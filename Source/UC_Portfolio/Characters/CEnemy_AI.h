#pragma once

#include "CoreMinimal.h"
#include "Characters/CEnemy.h"
#include "GenericTeamAgentInterface.h"
#include "CEnemy_AI.generated.h"

UCLASS()
class UC_PORTFOLIO_API ACEnemy_AI : public ACEnemy, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		uint8 TeamID = 1;


public:
	FORCEINLINE class UBehaviorTree* GetBehaviorTree() { return BehaviorTree; }
	FORCEINLINE uint8 GetTeamID() { return TeamID; }

#pragma region Inhretied via IGenericTeamAgentInterface
public:
	virtual FGenericTeamId GetGenericTeamId() const override;

#pragma endregion

public:
	ACEnemy_AI();
};
