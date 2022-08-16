#pragma once

#include "CoreMinimal.h"
#include "Actions/CSkillSideEffect.h"
#include "CSkillSideEffect_Projectile.generated.h"

UCLASS()
class UC_PORTFOLIO_API ACSkillSideEffect_Projectile : public ACSkillSideEffect
{
	GENERATED_BODY()
	
public:
	ACSkillSideEffect_Projectile();

protected:
	virtual void BeginPlay() override;

protected:
	virtual void PlayParticles() {};

protected:
	virtual void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnDestruction(AActor* DestroyedActor) override;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Scene;

	UPROPERTY(VisibleDefaultsOnly)
		class UProjectileMovementComponent* Projectile;

	UPROPERTY(EditDefaultsOnly)
		float ThrowPower = 1000.0f;
	
};
