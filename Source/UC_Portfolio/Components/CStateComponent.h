#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStateComponent.generated.h"

UENUM(BlueprintType)
enum class EStateType : uint8
{
	Idle, Roll, Teleport, Backstep, Action, Hitted, AirHitted, FallingEnd, Stun, Execution, Dodge, Block, Dead, Max,
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStateTypeChanged, EStateType, InPrevType, EStateType, InNewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UC_PORTFOLIO_API UCStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsIdleMode() { return Type == EStateType::Idle; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsRollMode() { return Type == EStateType::Roll; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsTeleport() { return Type == EStateType::Teleport; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsBackstepMode() { return Type == EStateType::Backstep; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsActionMode() { return Type == EStateType::Action; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsHittedMode() { return Type == EStateType::Hitted; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsAirHittedMode() { return Type == EStateType::AirHitted; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsFallingEndMode() { return Type == EStateType::FallingEnd; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsStunMode() { return Type == EStateType::Stun; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsExecutionMode() { return Type == EStateType::Execution; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsDodgeMode() { return Type == EStateType::Dodge; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsBlockMode() { return Type == EStateType::Block; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsDeadMode() { return Type == EStateType::Dead; }

public:	
	UCStateComponent();

	void SetIdleMode();
	void SetRollMode();
	void SetTeleportMode();
	void SetBackstepMode();
	void SetActionMode();
	void SetHittedMode();
	void SetDeadMode();
	void SetAirHitMode();
	void SetStunMode();
	void SetExecutionMode();
	void SetDodgeMode();
	void SetBlockMode();

protected:
	virtual void BeginPlay() override;		

private:
	void ChangeType(EStateType InNewType);

public:
	UPROPERTY(BlueprintAssignable)
		FStateTypeChanged OnStateTypeChanged;

private:
	EStateType Type;
};
