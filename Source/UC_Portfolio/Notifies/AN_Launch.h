#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_Launch.generated.h"

USTRUCT(BlueprintType)
struct FLauchInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		FVector Direction = FVector(1,0,0);
	UPROPERTY(EditAnywhere)
		float LauchPower = 350.0f;
};

UCLASS()
class UC_PORTFOLIO_API UAN_Launch : public UAnimNotify
{
	GENERATED_BODY()

public:
	FString GetNotifyName_Implementation() const override;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

public:
	UPROPERTY(EditAnywhere)
		FLauchInfo LauchInfo;
};