#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CLockOnComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UC_PORTFOLIO_API UCLockOnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCLockOnComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	const FORCEINLINE bool IsLockOn() const { return bLockOnBegin; }

public:
	void BeginLockOn();
	void EndLockOn();

private:
	UFUNCTION()
	void GetTargets();
	void SelectTarget();
	void ChangeTarget(bool InRight);
	void UpdateLockOn();
	void UpdateControllerRotation();
	void ShowOnLockOnWidget();
	void ShowOffLockOnWidget();

private:
	UPROPERTY(EditAnywhere)
		float TraceRadius = 1000.0f;

	UPROPERTY(EditAnywhere)
		float InteropSpeed = 2.5f;

	UPROPERTY(EditAnywhere)
		TEnumAsByte<EDrawDebugTrace::Type> DebugType;

private:
	TArray<class ACharacter*> Targets;
	class ACharacter* OwnerCharacter;
	class ACharacter* LockOnTarget;

private:
	bool bLockOnBegin = false;
};
