#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "COptionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UC_PORTFOLIO_API UCOptionComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
		float HorizontalLookRate = 45.0f;

	UPROPERTY(EditAnywhere)
		float VerticalLookRate = 45.0f;

private:
	UPROPERTY(EditAnywhere, Category = "DefaultSight")
		float SpringArmLength = 190.0f;

	UPROPERTY(EditAnywhere, Category = "DefaultSight")
		float Fov = 90.0f;

	UPROPERTY(EditAnywhere, Category = "DefaultSight")
		FVector SocketOffset = FVector::ZeroVector;

private:
	UPROPERTY(EditAnywhere, Category = "ZoomSight")
		float ZSpringArmLength = 180.0f;

	UPROPERTY(EditAnywhere, Category = "ZoomSight")
		float ZFov = 70.0f;

	UPROPERTY(EditAnywhere, Category = "ZoomSight")
		FVector ZSocketOffset = FVector(0.0f, 85.0f, 20.0f);

public:
	const FORCEINLINE float GetDefaultSprinArmLength() const { return SpringArmLength; }
	const FORCEINLINE float GetDefaultFov() const { return Fov; }
	const FORCEINLINE FVector GetDefaultSocketOffset() const { return SocketOffset; }

	const FORCEINLINE float GetZoomSprinArmLength() const { return ZSpringArmLength; }
	const FORCEINLINE float GetZoomFov() const { return ZFov; }
	const FORCEINLINE FVector GetZoomSocketOffset() const { return ZSocketOffset; }

public:
	FORCEINLINE float GetHorizontalLookRate() { return HorizontalLookRate; }
	FORCEINLINE float GetVerticalLookRate() { return VerticalLookRate; }

public:	
	UCOptionComponent();

protected:
	virtual void BeginPlay() override;		
};
