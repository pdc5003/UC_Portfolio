#include "CParabola.h"
#include "Global.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"

ACParabola::ACParabola()
{
	PrimaryActorTick.bCanEverTick = true;
	CHelpers::CreateComponent<UStaticMeshComponent>(this, &Cube, "Cube");
}

void ACParabola::BeginPlay()
{
	Super::BeginPlay();
	StartLocation = GetActorLocation();
}

void ACParabola::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateParabola();

}

void ACParabola::Initalize()
{
	bBeginUpdate = false;
	Cube->SetRelativeLocation(StartLocation);

	MaxHeightLocation.X = (FQuat(GetActorRotation()).GetForwardVector() + StartLocation).X;
	MaxHeightLocation.Y = (FQuat(GetActorRotation()).GetForwardVector() + StartLocation).Y;
	MaxHeightLocation.Z = UKismetMathLibrary::DegSin(Angle) + StartLocation.Z;

	Direction = (MaxHeightLocation - Cube->GetComponentLocation());
	Direction.Normalize();
	ProcessTime = 0.0f;
	GravityOffset = 0.0f;
	StartTime = GetWorld()->GetTimeSeconds();
}

void ACParabola::UpdateParabola()
{
	if (bBeginUpdate == false)
		return;

	ProcessTime = GetWorld()->GetTimeSeconds() - StartTime;
	GravityOffset += GetWorld()->GetTimeSeconds();
	float x = Direction.X * GetWorld()->GetDeltaSeconds() * Power * UKismetMathLibrary::DegCos(Angle);
	float y = Direction.Y * GetWorld()->GetDeltaSeconds() * Power * UKismetMathLibrary::DegCos(Angle);
	float z = Direction.Z * GetWorld()->GetDeltaSeconds() * Power +  ( -0.5f * FMath::Square(ProcessTime) * (Gravity + FMath::Clamp(GravityOffset, 0.0f, Gravity*1.5f)) );
	FVector current = Cube->GetComponentLocation();
	Cube->SetWorldLocation(FVector(current.X + x, current.Y + y, current.Z + z));

	
}

