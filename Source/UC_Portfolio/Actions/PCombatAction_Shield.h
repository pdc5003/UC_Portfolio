#pragma once

#include "CoreMinimal.h"
#include "Actions/PCombatAction_Melee.h"
#include "PCombatAction_Shield.generated.h"

UCLASS()
class UC_PORTFOLIO_API APCombatAction_Shield : public APCombatAction_Melee
{
	GENERATED_BODY()
	
public:
	APCombatAction_Shield();

protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;

public:
	// Combo, Aim,AttackEnd
	virtual void DoAction() override;
	virtual void End_DoAction() override;
	virtual void AbortDoAction() override;

public: // Inherited via IIMelee
	virtual void DoActionCancle() override;
	virtual void DoCounterAction() override;

public:
	virtual void OnBeginOverlap(class AActor* InAttacker, class AActor* InCauser, class AActor* InOther, const bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnEndOverlap(class AActor* InAttacker, class AActor* InAttackCauser, class AActor* InOther) override;


private:
	bool bCounterAttack;
	bool bPerfectBlocking;
	bool bBlockingTimingCheck;

private:
	float CounterAttackAbleTime = 0.35f;
	float BlockStartTime = 0.0f;
};
