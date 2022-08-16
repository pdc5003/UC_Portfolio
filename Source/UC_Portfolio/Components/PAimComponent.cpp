#include "Components/PAimComponent.h"
#include "Global.h"
#include "CHUD.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CStateComponent.h"
#include "Components/PCombatComponent.h"
#include "Components/COptionComponent.h"

UPAimComponent::UPAimComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}



void UPAimComponent::Initialize(ACharacter* InCharacter)
{
	OwnerCharacter = InCharacter;

	SpringArm = CHelpers::GetComponent<USpringArmComponent>(OwnerCharacter);
	Camera = CHelpers::GetComponent<UCameraComponent>(OwnerCharacter);
	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);

	Hud = OwnerCharacter->GetWorld()->GetFirstPlayerController()->GetHUD<ACHUD>();

	UPCombatComponent* combat = CHelpers::GetComponent<UPCombatComponent>(OwnerCharacter);
	combat->OnAiming.AddDynamic(this, &UPAimComponent::Aiming);

	UCOptionComponent* option = CHelpers::GetComponent<UCOptionComponent>(OwnerCharacter);
	ZoomOutTargetArmLength = option->GetDefaultSprinArmLength();
	ZooomOutFieldOfView = option->GetDefaultFov();
	ZoomOutSocketOffset = option->GetDefaultSocketOffset();
	
	ZoomInTargetArmLength = option->GetZoomSprinArmLength();
	ZooomInFieldOfView = option->GetZoomFov();
	ZoomInSocketOffset = option->GetZoomSocketOffset();
}

// Called every frame
void UPAimComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (OnAimingDelegate.IsBound())
		OnAimingDelegate.Execute();
}

void UPAimComponent::Aiming(bool bAiming)
{
	bAiming ? ZoomIn() : ZoomOut();
}

void UPAimComponent::ZoomIn()
{
	bAim = true;
	SpringArm->bEnableCameraLag = ZoomInEnableCameraLag;
	Transition = 0.0f;

	Hud->SetDraw();

	if (OnAimingDelegate.IsBound())
		OnAimingDelegate.Clear();

	OnAimingDelegate.BindDynamic(this, &UPAimComponent::ZoomInUpdate);
}

void UPAimComponent::ZoomOut()
{
	bAim = false;
	SpringArm->bEnableCameraLag = ZoomOutEnableCameraLag;
	Transition = 0.0f;

	Hud->SetNotDraw();

	if (OnAimingDelegate.IsBound())
		OnAimingDelegate.Clear();

	OnAimingDelegate.BindDynamic(this, &UPAimComponent::ZoomOutUpdate);
}

void UPAimComponent::ZoomInUpdate()
{
	if (Transition >= 1.0f)
	{
		OnAimingDelegate.Clear();
		return;
	}

	float armLength = SpringArm->TargetArmLength;
	SpringArm->TargetArmLength = UKismetMathLibrary::Lerp(armLength, ZoomInTargetArmLength, Transition);

	float fov = Camera->FieldOfView;
	Camera->FieldOfView = UKismetMathLibrary::Lerp(fov, ZooomInFieldOfView, Transition);

	FVector socketOffset = SpringArm->SocketOffset;
	SpringArm->SocketOffset = UKismetMathLibrary::VLerp(socketOffset, ZoomInSocketOffset, Transition);

	Transition += (1 / Duration) * GetWorld()->GetDeltaSeconds();

}

void UPAimComponent::ZoomOutUpdate()
{
	if (Transition >= 1.0f)
	{
		OnAimingDelegate.Clear();
		return;
	}

	float armLength = SpringArm->TargetArmLength;
	SpringArm->TargetArmLength = UKismetMathLibrary::Lerp(armLength, ZoomOutTargetArmLength, Transition);

	float fov = Camera->FieldOfView;
	Camera->FieldOfView = UKismetMathLibrary::Lerp(fov, ZooomOutFieldOfView, Transition);

	FVector socketOffset = SpringArm->SocketOffset;
	SpringArm->SocketOffset = UKismetMathLibrary::VLerp(socketOffset, ZoomOutSocketOffset, Transition);

	Transition += (1 / Duration) * GetWorld()->GetDeltaSeconds();
}

