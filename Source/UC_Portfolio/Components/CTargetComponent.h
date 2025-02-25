#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CTargetComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UC_PORTFOLIO_API UCTargetComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
		class UParticleSystem* Particle;

	UPROPERTY(EditAnywhere)
		float TraceRadius = 1000.0f;

	UPROPERTY(EditAnywhere)
		float InteropSpeed = 2.5f;

	UPROPERTY(EditAnywhere)
		TEnumAsByte<EDrawDebugTrace::Type> DebugType;

public:	
	UCTargetComponent();

public:
	void ToggleTarget();
	void ChangeTarget(bool InRight);

	void EndTargeting();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;	

private:
	void StartTargeting();

	void SetTraceTargets();
	void SetTarget();

	void ChangeCursor(class ACharacter* InCharacter);

private:
	class ACharacter* OwnerCharacter;
	class ACharacter* Target;
	class UParticleSystemComponent* Attached;

	TArray<class ACharacter*> TraceTargets;
};
