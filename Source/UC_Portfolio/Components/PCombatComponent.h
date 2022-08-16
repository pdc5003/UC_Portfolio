

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PCombatComponent.generated.h"

UENUM(BlueprintType)
enum class ECombatType : uint8
{
	AXE, BAREHAND, KNIFE, TWOHAND, SHIELD, KATANA, MAX,
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCombatTypeChange, ECombatType, InPrevType, ECombatType, InNewType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAiming, bool, bAim);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FThrowing, bool, bThrow);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UC_PORTFOLIO_API UPCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPCombatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly, Category = "DataAssets")
		class UPCombatData* DataAssets[(int32)ECombatType::MAX];

public:
	//AnimInstance Type Delegate
	UPROPERTY(BlueprintAssignable)
		FCombatTypeChange OnCombatTypeChange;

	//AnimInstance Type Delegate, AimComponent¿¡ ¿¬°á
	UPROPERTY(BlueprintAssignable)
		FAiming OnAiming;

	//AnimInstance Type Delegate
	UPROPERTY(BlueprintAssignable)
		FThrowing OnThrowing;

public:
	void GetBackCombatType();

public: //Attack Action, Aim
	void DoAction();
	void DoDashAction();
	void DoUpperAction();
	void DoSpecialAction();
	void DoSpecial2Action();
	void DoAim(bool InAim);
	void Dead();
	void End_Dead();
	void AbortAction();

public: //Shield Action
	UFUNCTION(BlueprintCallable)
	void SetShield();
	void BeginBlock();
	void EndBlock();
	void CounterAction();

public:
	void ActiveNiagaraTrail();
	void DeActiveNiagaraTrail();

public: //Hidden Weapon
	void SetHideWeapon(bool bHide);

public:
	UFUNCTION(BlueprintPure)
		FORCEINLINE class UPCombat* GetCurrent() const { return CombatDatas[(int32)Type]; }

public:
	class APArmed* GetAxe() const;

public: //Check CombatMode
	UFUNCTION(BlueprintPure)
		const FORCEINLINE bool IsAxe() const { return Type == ECombatType::AXE; }

	UFUNCTION(BlueprintPure)
		const FORCEINLINE bool IsBareHand() const { return Type == ECombatType::BAREHAND; }

	UFUNCTION(BlueprintPure)
		const FORCEINLINE bool IsKnife() const { return Type == ECombatType::KNIFE; }

	UFUNCTION(BlueprintPure)
		const FORCEINLINE bool IsTwoHand() const { return Type == ECombatType::TWOHAND; }

	UFUNCTION(BlueprintPure)
		const FORCEINLINE bool IsKatana() const { return Type == ECombatType::KATANA; }

public: //SetCombatMode
	UFUNCTION(BlueprintCallable)
		void SetAxeMode();
	UFUNCTION(BlueprintCallable)
		void SetBareHandMode();
	UFUNCTION(BlueprintCallable)
		void SetKnifeMode();
	UFUNCTION(BlueprintCallable)
		void SetTwoHandMode();
	UFUNCTION(BlueprintCallable)
		void SetKatanaMode();

public:
	FORCEINLINE void SetBlocking(bool* InBlocking) { bPerfectBlocking = InBlocking; }
	FORCEINLINE bool IsPerfectBlocking() { return bPerfectBlocking !=NULL ? *bPerfectBlocking : false; }

private: //Change Aremd, Change Mode
	void SetMode(ECombatType InType);
	void ChangeType(ECombatType InNewType);

private: //CombatType
	ECombatType Type;
	ECombatType Prev;
	UPROPERTY(EditFixedSize)
	class UPCombat* CombatDatas[(int32)ECombatType::MAX];

private: //Aim Share
	bool bAim;
	FORCEINLINE const bool* GetAimed() { return &bAim; }

private: //Perfect Blocking
	bool* bPerfectBlocking;

};
