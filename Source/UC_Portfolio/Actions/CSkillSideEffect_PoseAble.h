#pragma once

#include "CoreMinimal.h"
#include "Actions/CSkillSideEffect.h"
#include "CSkillSideEffect_PoseAble.generated.h"

/**
 * 
 */
UCLASS()
class UC_PORTFOLIO_API ACSkillSideEffect_PoseAble : public ACSkillSideEffect
{
	GENERATED_BODY()

public:
	ACSkillSideEffect_PoseAble();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UPoseableMeshComponent* PoseableMesh;

private:
	class ACharacter* OwnerCharacter;

private: //Material
	class UMaterialInstanceDynamic* GhostMaterial;
		
};
