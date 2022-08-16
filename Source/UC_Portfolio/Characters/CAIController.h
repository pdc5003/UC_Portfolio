#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CAIController.generated.h"

UCLASS()
class UC_PORTFOLIO_API ACAIController : public AAIController
{
	GENERATED_BODY()
	
private: //Range Area
	UPROPERTY(EditDefaultsOnly)
		float ActionRange = 300.0f;

	UPROPERTY(EditDefaultsOnly)
		float MiddleRange = 500.0f;

private: //Range Debug
	UPROPERTY(EditDefaultsOnly)
		bool bDrawDebug = true;

	UPROPERTY(EditDefaultsOnly)
		float AdjustCircleHeight = 50.0f;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UCBehaviorComponent* Behavior;

	UPROPERTY(VisibleDefaultsOnly)
		class UAIPerceptionComponent* Perception;

public:
	const FORCEINLINE float GetActionRange() const { return ActionRange; }
	const FORCEINLINE float GetMiddleRange() const { return MiddleRange; }
public:
	ACAIController();

	float GetSightRadius();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

private:
	UFUNCTION()
		void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);


private:
	class ACEnemy_AI* OwnerEnemy;
	class UAISenseConfig_Sight* Sight;

protected:
	UPROPERTY(BlueprintReadOnly)
	class APPlayer* targetPlayer;
};
