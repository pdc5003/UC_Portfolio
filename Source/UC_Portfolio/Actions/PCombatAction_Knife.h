#pragma once

#include "CoreMinimal.h"
#include "Actions/PCombatAction_Melee.h"
#include "PCombatAction_Knife.generated.h"

UCLASS()
class UC_PORTFOLIO_API APCombatAction_Knife : public APCombatAction_Melee
{
	GENERATED_BODY()
	
protected:
	virtual void DoSideEffect() override;
};
