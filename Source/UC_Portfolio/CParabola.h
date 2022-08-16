#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CParabola.generated.h"

UCLASS()
class UC_PORTFOLIO_API ACParabola : public AActor
{
	GENERATED_BODY()
	
public:	
	ACParabola();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION(BlueprintCallable)
		void Initalize();

	void UpdateParabola();

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent* Cube;
	
private:
	FVector StartLocation;
	FVector Direction;

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		bool bBeginUpdate = false;

private:
	UPROPERTY(EditAnywhere)
	FVector MaxHeightLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
		float Angle = 45.0f;

	UPROPERTY(EditAnywhere)
		float Gravity = 9.8f;

	float GravityOffset = 0.0f;

	UPROPERTY(EditAnywhere)
		float Power = 100.0f;

	UPROPERTY(EditAnywhere)
		float StartTime = 0.0f;
	UPROPERTY(EditAnywhere)
		float ProcessTime = 0.0f;


	
};
