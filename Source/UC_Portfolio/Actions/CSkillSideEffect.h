#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CSkillSideEffect.generated.h"

UCLASS()
class UC_PORTFOLIO_API ACSkillSideEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACSkillSideEffect();

protected:
	virtual void BeginPlay() override;

protected:
	virtual void PlayParticles() {};

protected:
	UFUNCTION()
		virtual void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {};
	UFUNCTION()
		virtual void OnDestruction(AActor* DestroyedActor) {};

protected:
	UPROPERTY(EditDefaultsOnly)
		float LifeSpanTime = 1.0f;

	
};
