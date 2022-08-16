#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CCamera.generated.h"

UCLASS()
class UC_PORTFOLIO_API ACCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	ACCamera();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Scene;

	UPROPERTY(VisibleDefaultsOnly)
		class UCameraComponent* Camera;
};
