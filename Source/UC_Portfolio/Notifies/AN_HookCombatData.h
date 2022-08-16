

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_HookCombatData.generated.h"

/**
 * 
 */
UCLASS()
class UC_PORTFOLIO_API UAN_HookCombatData : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	FString GetNotifyName_Implementation() const override;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

public:
	UPROPERTY(EditAnywhere)
		float Power = 5.0f;

	UPROPERTY(EditAnywhere)
		float HitStop = 0.0f;

	UPROPERTY(EditAnywhere)
		float Angle = 10.0f;

	UPROPERTY(EditAnywhere)
		float PushPower = 300.0f;

	UPROPERTY(EditAnywhere)
		int32 AttackStyle = 0;

};