#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_Axe_ThrowingEnd.generated.h"

/**
 * 
 */
UCLASS()
class UC_PORTFOLIO_API UAN_Axe_ThrowingEnd : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	FString GetNotifyName_Implementation() const override;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
