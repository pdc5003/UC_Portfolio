#include "Components/PAbilityComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/DefaultPawn.h"
#include "Components/COptionComponent.h"

UPAbilityComponent::UPAbilityComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UPAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	if (OwnerCharacter == nullptr)
		OwnerCharacter = Cast<ACharacter>(GetOwner());

}


// Called every frame
void UPAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPAbilityComponent::OnMoveForward(float InAxis)
{
	FRotator rotator = FRotator(0, OwnerCharacter->GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetForwardVector();

	OwnerCharacter->AddMovementInput(direction, InAxis);
}

void UPAbilityComponent::OnMoveRight(float InAxis)
{
	FRotator rotator = FRotator(0, OwnerCharacter->GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetRightVector();

	OwnerCharacter->AddMovementInput(direction, InAxis);
}

void UPAbilityComponent::OnHorizontalLook(float InAxis)
{
	float rate = CHelpers::GetComponent<UCOptionComponent>(OwnerCharacter)->GetHorizontalLookRate();

	OwnerCharacter->AddControllerYawInput(InAxis * rate * GetWorld()->GetDeltaSeconds());
}

void UPAbilityComponent::OnVerticalLook(float InAxis)
{
	float rate = CHelpers::GetComponent<UCOptionComponent>(OwnerCharacter)->GetVerticalLookRate();

	OwnerCharacter->AddControllerPitchInput(InAxis * rate * GetWorld()->GetDeltaSeconds());
}


