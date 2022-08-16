
#pragma once

#include "CoreMinimal.h"
#include "Actions/CSkillSideEffect.h"
#include "CSkillSideEffect_Around.generated.h"

UCLASS()
class UC_PORTFOLIO_API ACSkillSideEffect_Around : public ACSkillSideEffect
{
	GENERATED_BODY()
	
public:
	ACSkillSideEffect_Around();

protected:
	virtual void BeginPlay() override;

protected:
	virtual void PlayParticles() override;

protected:
	virtual void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnDestruction(AActor* DestroyedActor) override;


private:
	UPROPERTY(VisibleDefaultsOnly)
		class USphereComponent* Sphere;

	UPROPERTY(EditDefaultsOnly)
		class UNiagaraSystem* NiagaraParticle;

	UPROPERTY(EditDefaultsOnly)
		FVector ParticleScale = FVector::OneVector;

	UPROPERTY(EditDefaultsOnly)
		float Angle = 45.0f;

	UPROPERTY(EditDefaultsOnly)
		float PushPower = 1000;

	UPROPERTY(EditDefaultsOnly)
		float Power = 5.0f;
};
