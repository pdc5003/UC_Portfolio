#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IMelee.generated.h"

UINTERFACE(MinimalAPI)
class UIMelee : public UInterface
{
	GENERATED_BODY()
};

class UC_PORTFOLIO_API IIMelee
{
	GENERATED_BODY()

public:
	virtual void DoDashAction() = 0;
	virtual void DoUpperAction() = 0;
	virtual void DoSpecialAction() = 0;
	virtual void DoSpecial2Action() = 0;
	virtual void DoSideEffect() = 0;
	virtual void DoActionCancle() {};
	virtual void DoCounterAction() {};
};
