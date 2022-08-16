#include "PPlayer.h"
#include "Global.h"
#include "Actions/PCombat.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "Components/PAbilityComponent.h"
#include "Components/PCombatComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/SceneComponent.h"
#include "Components/PAimComponent.h"
#include "Components/COptionComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/PFinisherComponent.h"
#include "Components/CLockOnComponent.h"
#include "Components/ChildActorComponent.h"
#include "Components/CMontagesComponent.h"
#include "Characters/PCommandBuffer.h"
#include "Widgets/CUserWidget_Health.h"
#include "MyObject.h"

APPlayer::APPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm", GetMesh());
	CHelpers::CreateComponent<UCameraComponent>(this, &Camera, "Camera", SpringArm);

	CHelpers::CreateActorComponent<UPCombatComponent>(this, &Combat, "Combat");
	CHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State");
	CHelpers::CreateActorComponent<UCStatusComponent>(this, &Status, "Status");
	CHelpers::CreateActorComponent<UCOptionComponent>(this, &Option, "Option");
	CHelpers::CreateActorComponent<UPAbilityComponent>(this, &Ability, "Ability");
	CHelpers::CreateActorComponent<UCMontagesComponent>(this, &Montages, "Montages");
	CHelpers::CreateActorComponent<UPAimComponent>(this, &AimComp, "AimComp");
	CHelpers::CreateActorComponent<UPFinisherComponent>(this, &Finisher, "Finisher");
	CHelpers::CreateActorComponent<UCLockOnComponent>(this, &LockOn, "LockOn");

	bUseControllerRotationYaw = false;

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Character/Mesh/SK_Mannequin.SK_Mannequin'");
	GetMesh()->SetSkeletalMesh(mesh);

	TSubclassOf<UAnimInstance> animInstance;
	CHelpers::GetClass<UAnimInstance>(&animInstance, "AnimBlueprint'/Game/Player/ABP_Player.ABP_Player_C'");
	GetMesh()->SetAnimInstanceClass(animInstance);

	SpringArm->SetRelativeLocation(FVector(0, 0, 150));
	SpringArm->SetRelativeRotation(FRotator(0, 90, 0));
	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bEnableCameraLag = true;

	bUseControllerRotationYaw = true;

	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);
	GetCharacterMovement()->bOrientRotationToMovement = false;

	TSubclassOf<class UCUserWidget_Health> playerWidgetClass;

	CHelpers::GetClass<UMyObject>(&MyObject, "Blueprint'/Game/Sounds/BP_MyObject.BP_MyObject_C'");

}

void APPlayer::BeginPlay()
{
	//Material Setting
	//UMaterialInstanceConstant* body = Cast<UMaterialInstanceConstant>(GetMesh()->GetMaterial(0));
	//UMaterialInstanceConstant* logo = Cast<UMaterialInstanceConstant>(GetMesh()->GetMaterial(1));
	//
	//CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&body, "MaterialInstanceConstant'/Game/Character/Materials/MI_UE4Man_Body.MI_UE4Man_Body'");
	//CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&logo, "MaterialInstanceConstant'/Game/Materials/MI_Character_Logo.MI_Character_Logo'");



	BodyMaterial = UMaterialInstanceDynamic::Create(Body, this);
	LogoMaterial = UMaterialInstanceDynamic::Create(Logo, this);
	GetMesh()->SetMaterial(0, BodyMaterial);
	GetMesh()->SetMaterial(1, LogoMaterial);
	//Material Setting

	Super::BeginPlay();

	CommandBuffer = NewObject<UPCommandBuffer>();
	CommandBuffer->BeginPlay(this);

	Combat->SetShield();
	Combat->SetAxeMode();

	AimComp->Initialize(this);
	SpringArm->TargetArmLength = Option->GetDefaultSprinArmLength();
	Camera->FieldOfView = Option->GetDefaultFov();

	State->OnStateTypeChanged.AddDynamic(this, &APPlayer::OnStateTypeChanged);

	if (!!PlayerWidgetClass)
	{
		if (PlayerWidget == NULL)
		{
			PlayerWidget = CreateWidget<UCUserWidget_Health>(UGameplayStatics::GetPlayerController(GetWorld(), 0), PlayerWidgetClass);
			PlayerWidget->AddToViewport();
		}
	}

	PlayerWidget->HealthUpdate(Status->GetHealth(), Status->GetMaxHealth());
	HidePlayerHUD();


	//if (!!MyObject)
	//{
	//	MyTestObj = NewObject<UMyObject>();
	//	MyTestObj->PlayBgm(nullptr);
	//}

}

void APPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CommandBuffer->Tick(DeltaTime);
	UpdateAirBorne();

	CheckNull(State);
	if (State->IsIdleMode())
	{
		Status->AddHealth(DeltaTime* HealthRecoverPower);
		PlayerWidget->HealthUpdate(Status->GetHealth(), Status->GetMaxHealth());
	}

}

void APPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &APPlayer::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APPlayer::OnMoveRight);
	PlayerInputComponent->BindAxis("HorizontalLook", this, &APPlayer::OnHorizontalLook);
	PlayerInputComponent->BindAxis("VerticalLook", this, &APPlayer::OnVerticalLook);

	PlayerInputComponent->BindAction("Action", EInputEvent::IE_Pressed, this, &APPlayer::OnAction);
	PlayerInputComponent->BindAction("Axe", EInputEvent::IE_Pressed, this, &APPlayer::OnAxe);
	PlayerInputComponent->BindAction("BareHand", EInputEvent::IE_Pressed, this, &APPlayer::OnBareHand);
	PlayerInputComponent->BindAction("Aim", EInputEvent::IE_Pressed, this, &APPlayer::OnAim);
	PlayerInputComponent->BindAction("Aim", EInputEvent::IE_Released, this, &APPlayer::OffAim);

	PlayerInputComponent->BindAction("Finisher", EInputEvent::IE_Pressed, this, &APPlayer::OnFinisher);

	PlayerInputComponent->BindAction("LockOn", EInputEvent::IE_Pressed, this, &APPlayer::OnLockOn);
	PlayerInputComponent->BindAction("LockOn", EInputEvent::IE_Released, this, &APPlayer::OffLockOn);

	PlayerInputComponent->BindAction("Blocking", EInputEvent::IE_Pressed, this, &APPlayer::OnBlocking);
	PlayerInputComponent->BindAction("Blocking", EInputEvent::IE_Released, this, &APPlayer::OffBlocking);

	PlayerInputComponent->BindAction("Dodge", EInputEvent::IE_Pressed, this, &APPlayer::OnDodge);

}

void APPlayer::ShowPlayerHUD()
{
	if (!!PlayerWidget)
		PlayerWidget->SetVisibility(ESlateVisibility::Visible);
}

void APPlayer::HidePlayerHUD()
{
	if (!!PlayerWidget)
		PlayerWidget->SetVisibility(ESlateVisibility::Hidden);
}

void APPlayer::OnAction()
{
	CheckNull(Combat);

	if (bBlock == true)
	{
		Combat->CounterAction();
		return;
	}

	ECommandType type;
	CommandBuffer->CheckCommandList(type);

	if (type == ECommandType::Noraml || LockOn->IsLockOn() == false)
	{
		Combat->DoAction();
		return;
	}


	CheckFalse(LockOn->IsLockOn());

	CheckFalse(State->IsIdleMode());
	switch (type)
	{
		case ECommandType::Dash: Combat->DoDashAction(); break;
		case ECommandType::Upper: Combat->DoUpperAction(); break;
		case ECommandType::Special: Combat->DoSpecialAction(); break;
			//case ECommandType::Special2: Combat->DoSpecial2Action(); break;
	}
}

void APPlayer::OnAim()
{
	CheckFalse(State->IsIdleMode());
	Combat->DoAim(true);
}

void APPlayer::OffAim()
{
	CheckFalse(State->IsIdleMode());
	Combat->DoAim(false);
}

void APPlayer::OnFinisher()
{
	CheckFalse(State->IsIdleMode());

	if (Finisher->ExecutionFinisher() == true)
		State->SetActionMode();

	OffLockOn();
}

void APPlayer::OnLockOn()
{
	CheckNull(LockOn);
	CheckTrue(State->IsHittedMode());
	CheckTrue(State->IsAirHittedMode());
	LockOn->BeginLockOn();
}

void APPlayer::OffLockOn()
{
	CheckNull(LockOn);
	LockOn->EndLockOn();
}

void APPlayer::OnBlocking()
{
	if (State->IsIdleMode() == true)
	{
		CheckNull(Combat);
		Combat->BeginBlock();
	}
}

void APPlayer::OffBlocking()
{
	if (State->IsBlockMode() == true)
	{
		CheckNull(Combat);
		Combat->EndBlock();
	}
	bBlock = false;
}

void APPlayer::OnDodge()
{
	CheckFalse(State->IsIdleMode());
	CheckTrue(State->IsDodgeMode());

	float fwdDir = GetInputAxisValue("MoveForward");
	float rightDir = GetInputAxisValue("MoveRight");

	if (fwdDir > 0)
	{
		PlayAnimMontage(DodgeMontage[0]);
		State->SetDodgeMode();
		Status->SetStop();
		return;
	}

	if (fwdDir < 0)
	{
		PlayAnimMontage(DodgeMontage[1]);
		State->SetDodgeMode();
		Status->SetStop();
		return;
	}

	if (rightDir > 0)
	{
		PlayAnimMontage(DodgeMontage[2]);
		State->SetDodgeMode();
		Status->SetStop();
		return;
	}

	if (rightDir < 0)
	{
		PlayAnimMontage(DodgeMontage[3]);
		State->SetDodgeMode();
		Status->SetStop();
		return;
	}

}

void APPlayer::OnMoveForward(float InAxis)
{
	CheckFalse(Status->CanMove());

	Ability->OnMoveForward(InAxis);
	CommandBuffer->AddInputBuffer(InAxis);
}


void APPlayer::OnMoveRight(float InAxis)
{
	CheckFalse(Status->CanMove());
	Ability->OnMoveRight(InAxis);
}

void APPlayer::OnHorizontalLook(float InAxis)
{
	CheckTrue(State->IsHittedMode());
	CheckTrue(State->IsAirHittedMode());
	Ability->OnHorizontalLook(InAxis);
}

void APPlayer::OnVerticalLook(float InAxis)
{
	CheckTrue(State->IsHittedMode());
	CheckTrue(State->IsAirHittedMode());
	Ability->OnVerticalLook(InAxis);
}

void APPlayer::OnAxe()
{
	Combat->SetAxeMode();
}

void APPlayer::OnBareHand()
{
	Combat->SetBareHandMode();
}


void APPlayer::Launch_Character(FVector InDirection, float InPower)
{
	if (InDirection.IsNearlyZero() == true)
		return;

	FVector FDirection = FQuat(FRotator(GetActorRotation())).GetForwardVector() * InDirection.X;
	FVector RDirection = FQuat(FRotator(GetActorRotation())).GetRightVector() * InDirection.Y;
	FVector UDirection = FQuat(FRotator(GetActorRotation())).GetUpVector() * InDirection.Z;

	FVector result = FDirection + RDirection + UDirection;
	LaunchCharacter(result * InPower, true, true);
	return;
}

int APPlayer::SendDamage(FDoCombatData const& combatData, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	DamageInstigator = EventInstigator;

	if (State->IsDeadMode())
		return -1;

	if (State->IsDodgeMode())
		return -1;

	if (State->IsExecutionMode())
		return -1;

	FVector dirToTarget = (EventInstigator->GetPawn()->GetActorLocation() - GetActorLocation()).GetSafeNormal();

	//Check Blocking Angle
	{
		if (!!EventInstigator && State->IsBlockMode())
		{

			FVector fwd = FQuat(GetActorRotation()).GetForwardVector();
			if (FVector::DotProduct(fwd, dirToTarget) > UKismetMathLibrary::DegCos(85.0f))
			{
				bBlock = true;
				OnBlockingSideEffect();
			}
			else
				bBlock = false;
		}
	}

	if (bBlock == true)
		return 1;

	bool bAirBorne = BeginAirBorne(combatData, dirToTarget);

	if (bAirBorne == true)
	{
		//LookAtAttacker
		{
			if (IsValid(DamageInstigator))
			{
				FVector start = GetActorLocation();
				FVector target = DamageInstigator->GetPawn()->GetActorLocation();
				FRotator rotator = UKismetMathLibrary::FindLookAtRotation(start, target);
				GetController()->SetControlRotation(FRotator(0, rotator.Yaw, 0));
			}
		}
	}
	else
	{
		LaunchCharacter(-dirToTarget * combatData.PushPower, false, true);
		State->SetHittedMode();
	}

	Status->SubHealth(combatData.Power);
	PlayerWidget->HealthUpdate(Status->GetHealth(), Status->GetMaxHealth());

	return 0;
}

void APPlayer::Hit_End()
{
	bSuperArmor = false;
	State->SetIdleMode();
	Status->SetMove();
}

void APPlayer::Set_SuperArmor(bool InSuperArmor)
{
	bSuperArmor = InSuperArmor;
}

FGenericTeamId APPlayer::GetGenericTeamId() const
{
	return TeamID;
}

void APPlayer::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
		case EStateType::Hitted: Hitted(); break;
		case EStateType::AirHitted: AirHitted(); break;
		case EStateType::Stun: Stunning(); break;
		case EStateType::Dead: Dead(); break;
	}
}

void APPlayer::Hitted()
{
	CheckTrue(bSuperArmor);
	bBlock = false;
	Status->SetStop();
	Montages->PlayHitted();
	Combat->AbortAction();
}

void APPlayer::AirHitted()
{
	CheckTrue(bSuperArmor);
	bAirHit = true;
	Status->SetStop();
	Montages->PlayAirBorne();
}

void APPlayer::Dead()
{
}

void APPlayer::Stunning()
{
}



void APPlayer::OnBlockingSideEffect()
{
	Launch_Character(FVector(-1, 0, 0), GuardBackPower);

	if (!!BlockingShake)
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerCameraManager->StartCameraShake(BlockingShake);

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1e-3f);
	UKismetSystemLibrary::K2_SetTimer(this, "ReleaseBlockingHitStop", BlockingHitStop * 1e-3f, false);
}

void APPlayer::ReleaseBlockingHitStop()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
}


bool APPlayer::BeginAirBorne(FDoCombatData const& IncombatData, FVector const& InDirection)
{
	if (bSuperArmor == true)
		return false;



	FlightTime = 0.0f;
	FVector airVelocity = FVector::ZeroVector;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);

	if (State->IsAirHittedMode() && GetCharacterMovement()->IsFalling() == true)
	{
		if (IncombatData.AttackStyle == EAttackStyle::Upper)
		{
			GetCharacterMovement()->GravityScale = 0.98f;
			airVelocity.X = -InDirection.X * IncombatData.PushPower * 1e-3;
			airVelocity.Y = -InDirection.Y * IncombatData.PushPower * 1e-3;
			airVelocity.Z = UKismetMathLibrary::DegSin(IncombatData.Angle) * IncombatData.PushPower - FMath::Abs(GetVelocity().Z) * 0.15f;

			LaunchCharacter(airVelocity, false, true);

			State->SetAirHitMode();
			bAirHit = true;
			return true;
		}

		if (IncombatData.AttackStyle == EAttackStyle::Blast)
		{
			GetCharacterMovement()->GravityScale = 0.98f;
			airVelocity.X = -InDirection.X * IncombatData.PushPower * 3.0f;
			airVelocity.Y = -InDirection.Y * IncombatData.PushPower * 3.0f;
			airVelocity.Z = UKismetMathLibrary::DegSin(IncombatData.Angle) * IncombatData.PushPower * 2.0f;

			LaunchCharacter(airVelocity, false, true);

			State->SetAirHitMode();
			bAirHit = true;
			return true;
		}

		GetCharacterMovement()->GravityScale = 0.98f;
		airVelocity.X = -InDirection.X * IncombatData.PushPower * 0.055f;
		airVelocity.Y = -InDirection.Y * IncombatData.PushPower * 0.055f;
		airVelocity.Z = UKismetMathLibrary::DegSin(IncombatData.Angle) * IncombatData.PushPower;

		LaunchCharacter(airVelocity, false, true);

		State->SetAirHitMode();
		bAirHit = true;
		return true;
	}

	if (IncombatData.AttackStyle == EAttackStyle::Upper)
	{
		GetCharacterMovement()->GravityScale = 0.98f;
		airVelocity.X = -InDirection.X * IncombatData.PushPower * 1e-3;
		airVelocity.Y = -InDirection.Y * IncombatData.PushPower * 1e-3;
		airVelocity.Z = UKismetMathLibrary::DegSin(IncombatData.Angle) * IncombatData.PushPower;
		LaunchCharacter(airVelocity, false, true);

		State->SetAirHitMode();
		bAirHit = true;
		return true;
	}

	return false;
}

void APPlayer::UpdateAirBorne()
{
	if (GetCharacterMovement()->IsFalling() == false && bAirHit == true)
	{
		bAirHit = false;
		bSuperArmor = true;
		GetCharacterMovement()->GravityScale = 1.0f;
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		StopAnimMontage();
		Montages->PlayAirFalling();
		return;
	}

	if (bAirHit == true)
	{
		FlightTime += GetWorld()->GetDeltaSeconds();
		GetCharacterMovement()->GravityScale += (0.5f * InAirGravity * FMath::Square(FlightTime));
		return;
	}

}




