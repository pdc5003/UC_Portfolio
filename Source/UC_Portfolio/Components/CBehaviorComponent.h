#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CBehaviorComponent.generated.h"

UENUM(BlueprintType)
enum class EBehaviorType : uint8
{
	Idle, Trace, Roaming, Teleport, Action, DashAction, UpperAction, Hitted, Avoid, Dead, MAX
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBehaviorTypeChanged, EBehaviorType, InPrevType, EBehaviorType, InNewType);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UC_PORTFOLIO_API UCBehaviorComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
		FName BehaviorKey = "Behavior";

	UPROPERTY(EditAnywhere)
		FName PlayerKey = "Player";

	UPROPERTY(EditAnywhere)
		FName WarpLocationKey = "WarpLocation";

public:
	UFUNCTION(BlueprintPure)
		bool IsIdleMode();

	UFUNCTION(BlueprintPure)
		bool IsTraceMode();

	UFUNCTION(BlueprintPure)
		bool IsActionMode();

	UFUNCTION(BlueprintPure)
		bool IsDashActionMode();

	UFUNCTION(BlueprintPure)
		bool IsUpperActionMode();

	UFUNCTION(BlueprintPure)
		bool IsHittedMode();

	UFUNCTION(BlueprintPure)
		bool IsAvoidMode();

	UFUNCTION(BlueprintPure)
		bool IsDeadMode();

	UFUNCTION(BlueprintPure)
		bool IsRoamingMode();

	UFUNCTION(BlueprintPure)
		bool IsTeleportMode();


	FORCEINLINE void SetBlackboard(class UBlackboardComponent* InBlackboard) { Blackboard = InBlackboard; }

public:
	UCBehaviorComponent();

public:
	UFUNCTION()
	void SetIdleMode();
	UFUNCTION()
	void SetTraceMode();
	UFUNCTION()
	void SetActionMode();
	UFUNCTION()
	void SetDashActionMode();
	UFUNCTION()
	void SetUpperActionMode();
	UFUNCTION()
	void SetHittedMode();
	UFUNCTION()
	void SetAvoidMode();
	UFUNCTION()
	void SetDeadMode();
	UFUNCTION()
	void SetRoamingMode();
	UFUNCTION()
	void SetTeleportMode();

	class APPlayer* GetTargetPlayer();

protected:
	virtual void BeginPlay() override;

private:
	void ChangeType(EBehaviorType InType);
	EBehaviorType GetType();

public:
	UPROPERTY(BlueprintAssignable)
		FBehaviorTypeChanged OnBehaviorTypeChanged;

private:
	class UBlackboardComponent* Blackboard;
};
