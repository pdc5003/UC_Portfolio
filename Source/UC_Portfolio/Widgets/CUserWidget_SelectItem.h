#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_SelectItem.generated.h"

UCLASS()
class UC_PORTFOLIO_API UCUserWidget_SelectItem : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintCallable)
		void Click();

	UFUNCTION(BlueprintCallable)
		void Hover();

	UFUNCTION(BlueprintCallable)
		void Unhover();

private:
	class UCUserWidget_Select* GetSelectWidget();
};
