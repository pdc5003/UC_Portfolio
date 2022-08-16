

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CExecutionWidget.generated.h"

/**
 * 
 */
UCLASS()
class UC_PORTFOLIO_API UCExecutionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
		void ShowExecutionIcon();

	UFUNCTION(BlueprintImplementableEvent)
		void HideExecutionIcon();
};
