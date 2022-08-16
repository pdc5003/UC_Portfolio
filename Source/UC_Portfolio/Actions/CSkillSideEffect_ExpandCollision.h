

#pragma once

#include "CoreMinimal.h"
#include "Actions/CSkillSideEffect.h"
#include "CSkillSideEffect_ExpandCollision.generated.h"

/**
 * 
 */
UCLASS()
class UC_PORTFOLIO_API ACSkillSideEffect_ExpandCollision : public ACSkillSideEffect
{
	GENERATED_BODY()
	
public:
	ACSkillSideEffect_ExpandCollision();

protected:
	virtual void BeginPlay() override;

protected:
	virtual void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnDestruction(AActor* DestroyedActor) override;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Scene;

	UPROPERTY(EditDefaultsOnly)
		class UBoxComponent* Box;
};
