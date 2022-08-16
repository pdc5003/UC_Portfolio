#include "Actions/PArmed_Shield.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/ShapeComponent.h"

void APArmed_Shield::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(OwnerCharacter == OtherActor);

	FHitResult result = SweepResult;
	if (!!currentShapeComponent)
	{
		FVector right = FQuat(currentShapeComponent->GetComponentRotation()).GetRightVector();
		result.Location = currentShapeComponent->GetComponentLocation() + (right * HitOffset);
	}

	if (OnArmedBeginOverlap.IsBound())
		OnArmedBeginOverlap.Broadcast(OwnerCharacter, this, OtherActor, bFromSweep, result);

}
