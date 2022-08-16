#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AngleActor.generated.h"

UCLASS()
class UC_PORTFOLIO_API AAngleActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAngleActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, Category = "Actors")
		class AActor* Ball;

	UPROPERTY(EditAnywhere, Category = "Actors")
		class AActor* Wind;

private:
	void CalAngle();
	void CalAngle2D();

};
