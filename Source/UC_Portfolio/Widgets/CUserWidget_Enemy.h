#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_Enemy.generated.h"
UCLASS()
class UC_PORTFOLIO_API UCUserWidget_Enemy : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
		void BeginLockOn();

	UFUNCTION(BlueprintImplementableEvent)
		void EndLockOn();

	UFUNCTION(BlueprintImplementableEvent)
		void HealthUpdate(float InHealth, float InMaxHealth);

	UFUNCTION(BlueprintImplementableEvent)
		void StunGauageUpdate(float InStun, float InMaxStun);

};
