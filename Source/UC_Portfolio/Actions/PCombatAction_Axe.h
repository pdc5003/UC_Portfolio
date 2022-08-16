

#pragma once

#include "CoreMinimal.h"
#include "Actions/PCombatAction_Melee.h"
#include "PCombatAction_Axe.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FThrowing, bool, bThrow);

UCLASS()
class UC_PORTFOLIO_API APCombatAction_Axe : public APCombatAction_Melee
{
	GENERATED_BODY()

public:
	APCombatAction_Axe();

protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;


public:
	// Combo, Aim,AttackEnd
	virtual void DoAction() override;
	virtual void Next_DoAction() override;
	virtual void End_DoAction() override;

	virtual void OnAim() override;
	virtual void OffAim() override;

	virtual void DoDashAction() override;
	virtual void DoUpperAction() override;
	virtual void DoSpecialAction() override;
	virtual void DoSpecial2Action() override;

public:
	virtual void OnBeginOverlap(class AActor* InAttacker, class AActor* InCauser, class AActor* InOther, const bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnEndOverlap(class AActor* InAttacker, class AActor* InAttackCauser, class AActor* InOther) override;

private:
	void DoAxeAction();

private:
	//AimMode
	bool bAim;
	
};
