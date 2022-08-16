#pragma once

#include "CoreMinimal.h"
#include "Actions/PArmed.h"
#include "PArmed_Shield.generated.h"

/**
 * 
 */
UCLASS()
class UC_PORTFOLIO_API APArmed_Shield : public APArmed
{
	GENERATED_BODY()
	

protected: //OverlapEvent
	virtual void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

};
