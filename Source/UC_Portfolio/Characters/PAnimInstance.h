

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Components/PCombatComponent.h"
#include "Actions/PCombatAction_Axe.h"
#include "PAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UC_PORTFOLIO_API UPAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected: // Speed, Direction
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float Speed;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float Direction;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool bAiming;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool bThrowing;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		ECombatType CombatType;

	/* 점프 후 떨어지는 상태 체크 변수 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool bFalling;

private:
	UFUNCTION()
		void OnAiming(bool bAim);

	UFUNCTION()
		void OnThrow(bool bThrow);

private:
	UFUNCTION()
		void OnActionTypeChanged(ECombatType InPrevType, ECombatType InNewType);
};
