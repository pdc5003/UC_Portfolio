

#pragma once

#include "CoreMinimal.h"
#include "Actions/PCombatAction.h"
#include "PCombatAction_Range.generated.h"

/**
 * 
 */
UCLASS()
class UC_PORTFOLIO_API APCombatAction_Range : public APCombatAction
{
	GENERATED_BODY()
	
public:
	APCombatAction_Range();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	FORCEINLINE void EnableCombo() { bEnable = true; }
	FORCEINLINE void DisableCombo() { bEnable = false; }

public:
	// Combo, AttackEnd
	virtual void DoAction() override;
	virtual void Next_DoAction() override;
	virtual void End_DoAction() override;

	virtual void OnAim() override;
	virtual void OffAim() override;

protected:
	//Combo Index, Enable NextCombo, Exit Combo, HitOtherCharacters
	int32 Index;
	bool bEnable;
	bool bExist;

};
