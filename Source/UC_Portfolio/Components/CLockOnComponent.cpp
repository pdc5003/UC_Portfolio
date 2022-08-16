#include "Components/CLockOnComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/WidgetComponent.h"
#include "Widgets/CUserWidget_Enemy.h"

UCLockOnComponent::UCLockOnComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UCLockOnComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());
}

void UCLockOnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdateLockOn();
}

void UCLockOnComponent::BeginLockOn()
{
	EndLockOn();
	UKismetSystemLibrary::K2_ClearTimer(this, "GetTargets");
	UKismetSystemLibrary::K2_SetTimer(this, "GetTargets", 0.1f, false);
	bLockOnBegin = true;
	
}

void UCLockOnComponent::EndLockOn()
{
	bLockOnBegin = false;
	
	ShowOffLockOnWidget();

	LockOnTarget = NULL;
	Targets.Empty();

}

void UCLockOnComponent::GetTargets()
{
	FVector start = OwnerCharacter->GetActorLocation();
	FVector end = FVector(start.X, start.Y, start.Z + 1);

	TArray<AActor*> ignoreActors;
	ignoreActors.Add(OwnerCharacter);

	TArray<FHitResult> hitResults;
	UKismetSystemLibrary::SphereTraceMultiByProfile(GetWorld(), start, end, TraceRadius, "Pawn", false, ignoreActors, DebugType, hitResults, true, FLinearColor::Green, FLinearColor::Red, 1.0f);

	for (const FHitResult& hitResult : hitResults)
	{
		if (hitResult.GetActor()->GetClass() == OwnerCharacter->GetClass())
			continue;

		ACharacter* character = Cast<ACharacter>(hitResult.GetActor());

		if (!!character)
			Targets.AddUnique(character);
	}

	if (Targets.Num() <= 0)
	{
		EndLockOn();
		return;
	}

	SelectTarget();
}

void UCLockOnComponent::SelectTarget()
{
	CheckFalse(bLockOnBegin);
	CheckTrue(Targets.Num()<=0);

	float angle = -2.0f;
	float distance = 100000.0f;
	ACharacter* target = NULL;

	for (ACharacter* character : Targets)
	{
		FVector direction = FQuat(OwnerCharacter->GetControlRotation()).GetForwardVector();
		FVector offset = character->GetActorLocation() - OwnerCharacter->GetActorLocation();
		float temp = direction | offset.GetSafeNormal();

		if (temp < angle)
			continue;

		if (offset.Size() > distance)
			continue;

		angle = temp;
		distance = offset.Size();
		target = character;
	}
	
	CheckNull(target);

	LockOnTarget = target;
	ShowOnLockOnWidget();
}

void UCLockOnComponent::ChangeTarget(bool InRight)
{
	CheckNull(LockOnTarget);

	TMap<float, ACharacter*> map;
	for (ACharacter* character : Targets)
	{
		if (LockOnTarget == character)
			continue;

		FVector targetLocation = character->GetActorLocation();
		FVector ownerLocation = OwnerCharacter->GetActorLocation();
		FVector ownerToTaget = targetLocation - ownerLocation;

		FQuat quat = FQuat(OwnerCharacter->GetControlRotation());
		FVector forward = quat.GetForwardVector();
		FVector up = quat.GetUpVector();

		FVector cross = forward ^ ownerToTaget;
		float dot = cross | up;

		map.Add(dot, character);
	}


	float minimum = FLT_MAX;
	ACharacter* target = NULL;

	TArray<float> keys;
	map.GetKeys(keys);
	for (float key : keys)
	{
		if (InRight == true)
		{
			if (key < 0.0f)
				continue;
		}
		else
		{
			if (key > 0.0f)
				continue;
		}

		if (FMath::Abs(key) > minimum)
			continue;

		target = *map.Find(key);
		minimum = FMath::Abs(key);
	}
	ShowOffLockOnWidget();
	LockOnTarget = target;
	ShowOnLockOnWidget();
}

void UCLockOnComponent::UpdateLockOn()
{
	CheckFalse(bLockOnBegin);
	CheckNull(LockOnTarget);

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(LockOnTarget);

	CheckNull(state);

	bool b = false;
	b |= state->IsDeadMode();
	b |= state->IsExecutionMode();

	if (b == true)
	{
		EndLockOn();
		return;
	}

	UpdateControllerRotation();
}

void UCLockOnComponent::UpdateControllerRotation()
{
	FVector start = OwnerCharacter->GetActorLocation();
	FVector target = LockOnTarget->GetActorLocation();

	FRotator rotator = UKismetMathLibrary::FindLookAtRotation(start, target);
	FRotator current = OwnerCharacter->GetControlRotation();

	rotator = UKismetMathLibrary::RInterpTo(current, rotator, GetWorld()->GetDeltaSeconds(), InteropSpeed);
	
	FRotator controllerRotator = OwnerCharacter->GetController()->GetControlRotation();

	OwnerCharacter->GetController()->SetControlRotation(FRotator(controllerRotator.Pitch, rotator.Yaw, rotator.Roll));
}

void UCLockOnComponent::ShowOnLockOnWidget()
{
	CheckFalse(bLockOnBegin);
	CheckNull(LockOnTarget);

	UWidgetComponent* widget = CHelpers::GetComponent<UWidgetComponent>(LockOnTarget);
	if (!!widget)
		Cast<UCUserWidget_Enemy>(widget->GetUserWidgetObject())->BeginLockOn();
}

void UCLockOnComponent::ShowOffLockOnWidget()
{
	CheckNull(LockOnTarget);
	UWidgetComponent* widget = CHelpers::GetComponent<UWidgetComponent>(LockOnTarget);
	if (!!widget)
		Cast<UCUserWidget_Enemy>(widget->GetUserWidgetObject())->EndLockOn();
}

