#pragma once

#include "CoreMinimal.h"
#include "Actions/PCombatAction.h"
#include "Interface/IMelee.h"
#include "PCombatAction_Melee.generated.h"

UCLASS()
class UC_PORTFOLIO_API APCombatAction_Melee : public APCombatAction, public IIMelee
{
	GENERATED_BODY()

public:
	APCombatAction_Melee();

protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE void EnableCombo() { bEnable = true; }
	FORCEINLINE void DisableCombo() { bEnable = false; }
	FORCEINLINE bool IsEnableCombo() { return bEnable; }

public:
	// Combo, AttackEnd
	virtual void DoAction() override;
	virtual void Next_DoAction() override;
	virtual void End_DoAction() override;
	virtual void AbortDoAction() override;
	virtual void HookCurrentCombatData(const float& Power, const float& HitStop, const float& Angle, const float& PushPower, int32 AttackType) override;


	// Inherited via IIMelee
	virtual void DoDashAction() override;
	virtual void DoUpperAction() override;
	virtual void DoSpecialAction() override;
	virtual void DoSpecial2Action() override;
	virtual void DoSideEffect() override;

public:
	virtual void OnBeginOverlap(class AActor* InAttacker, class AActor* InCauser, class AActor* InOther, const bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnEndOverlap(class AActor* InAttacker, class AActor* InAttackCauser, class AActor* InOther) override;



protected:
	//Combo Index, Enable NextCombo, Exit Combo, HitOtherCharacters
	int32 Index;
	bool bEnable;
	bool bExist;
	TArray<class ACharacter*> HittedCharacter;

protected:
	UPROPERTY(EditDefaultsOnly)
		class UParticleSystem* PerfectBlockParticle;

	UPROPERTY(EditDefaultsOnly)
		class UParticleSystem* NormalBlockParticle;

	UPROPERTY(EditDefaultsOnly)
		float BlockParticleOffset = 15.0f;

protected:
	int SendDamageSuccess = -1;

private:
	//ReturnGlobalTime
	UFUNCTION()
		void ReturnToGlobalDilation();

};
