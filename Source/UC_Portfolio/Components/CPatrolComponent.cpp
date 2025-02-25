#include "CPatrolComponent.h"
#include "Global.h"
#include "Actors/CPatrolPath.h"
#include "Components/SplineComponent.h"

UCPatrolComponent::UCPatrolComponent()
{

}

bool UCPatrolComponent::GetMoveTo(FVector& OutLocation, float& OutAcceptanceRaidus)
{
	OutLocation = FVector::ZeroVector;
	OutAcceptanceRaidus = AcceptanceRadius;
	CheckNullResult(Path, false);

	OutLocation = Path->GetSpline()->GetLocationAtSplinePoint(Index, ESplineCoordinateSpace::World);
	return true;
}

void UCPatrolComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UCPatrolComponent::UpdateNextIndex()
{
	CheckNull(Path);

	int32 count = Path->GetSpline()->GetNumberOfSplinePoints();
	if (bReverse)
	{
		if (Index > 0)
		{
			Index--;

			return;
		}

		if (Path->GetSpline()->IsClosedLoop())
		{
			Index = count - 1;

			return;
		}

		Index = 1;
		bReverse = false;

		return;
	}

	if (Index < count - 1)
	{
		Index++;

		return;
	}

	if (Path->GetSpline()->IsClosedLoop())
	{
		Index = 0;

		return;
	}

	Index = count - 2;
	bReverse = true;
}