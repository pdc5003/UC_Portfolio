#pragma once

#include "CoreMinimal.h"
#include "Actions/PCombatAction_Melee.h"
#include "PCombatAction_Katana.generated.h"

UENUM()
enum class AttackFlag : uint8
{
	DefaultAtk = 1U << 0,
	DashAtk = 1U << 1,
	UpperAtk = 1U << 2
};

UCLASS()
class UC_PORTFOLIO_API APCombatAction_Katana : public APCombatAction_Melee
{
	GENERATED_BODY()

public:
	APCombatAction_Katana();

protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;


public:
	// Combo, Aim,AttackEnd
	virtual void DoAction() override;
	virtual void Next_DoAction() override;
	virtual void End_DoAction() override;
	virtual void AbortDoAction() override;

	virtual void DoDashAction() override;
	virtual void DoUpperAction() override;
	virtual void DoSpecialAction() override;
	virtual void DoSpecial2Action() override;

public:
	virtual void OnBeginOverlap(class AActor* InAttacker, class AActor* InCauser, class AActor* InOther, const bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnEndOverlap(class AActor* InAttacker, class AActor* InAttackCauser, class AActor* InOther) override;

private:
	FORCEINLINE void OnDefaultAtkFlag() { AtkFlag |= (UINT)AttackFlag::DefaultAtk; }
	FORCEINLINE void OnDashAtkFlag() { AtkFlag |= (UINT)AttackFlag::DashAtk; }
	FORCEINLINE void OnUpperAtkFlag() { AtkFlag |= (UINT)AttackFlag::UpperAtk; }

	FORCEINLINE bool IsOnDefaultFlag() {return (AtkFlag & (UINT)AttackFlag::DefaultAtk);}
	FORCEINLINE bool IsOnDashAtkFlag() { return (AtkFlag & (UINT)AttackFlag::DashAtk); }
	FORCEINLINE bool IsOnUpperAtkFlag() { return (AtkFlag & (UINT)AttackFlag::UpperAtk); }

private:
	UINT AtkFlag = 0U;

private:
	bool bHit = false;
};
