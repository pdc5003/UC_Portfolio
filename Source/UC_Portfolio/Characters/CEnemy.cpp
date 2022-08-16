#include "CEnemy.h"
#include "Global.h"
#include "Actions/PCombat.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/PCombatComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/WidgetComponent.h"
#include "Widgets/CUserWidget_Enemy.h"
#include "Widgets/CExecutionWidget.h"
#include "AIController.h"

ACEnemy::ACEnemy()
{
	CHelpers::CreateComponent<UWidgetComponent>(this, &EnemyWidget, "EnemyWidget", GetMesh());
	CHelpers::CreateComponent<UWidgetComponent>(this, &ExecutionWidget, "ExecutionWidget", GetMesh());


	CHelpers::CreateActorComponent<UPCombatComponent>(this, &Combat, "Combat");
	CHelpers::CreateActorComponent<UCMontagesComponent>(this, &Montages, "Montages");
	CHelpers::CreateActorComponent<UCStatusComponent>(this, &Status, "Status");
	CHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State");


	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Character/Mesh/SK_Mannequin.SK_Mannequin'");
	GetMesh()->SetSkeletalMesh(mesh);

	TSubclassOf<UAnimInstance> animInstance;
	CHelpers::GetClass<UAnimInstance>(&animInstance, "AnimBlueprint'/Game/Enemies/ABP_Enemy.ABP_Enemy_C'");
	GetMesh()->SetAnimInstanceClass(animInstance);

	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);
	GetCharacterMovement()->MaxWalkSpeed = Status->GetRunSpeed();

	TSubclassOf<UCUserWidget_Enemy> enemyWidgetClass;
	CHelpers::GetClass<UCUserWidget_Enemy>(&enemyWidgetClass, "WidgetBlueprint'/Game/Widgets/WB_Enemy.WB_Enemy_C'");
	EnemyWidget->SetWidgetClass(enemyWidgetClass);
	EnemyWidget->SetRelativeLocation(FVector(0, 0, 275));
	EnemyWidget->SetDrawSize(FVector2D(240, 30));
	EnemyWidget->SetWidgetSpace(EWidgetSpace::Screen);

	TSubclassOf<UCExecutionWidget> executionWidgetClass;
	CHelpers::GetClass<UCExecutionWidget>(&executionWidgetClass, "WidgetBlueprint'/Game/Widgets/WB_Execution.WB_Execution_C'");
	ExecutionWidget->SetWidgetClass(executionWidgetClass);
	ExecutionWidget->SetRelativeLocation(FVector(0, 0, 190));
	ExecutionWidget->SetDrawSize(FVector2D(120, 120));
	ExecutionWidget->SetWidgetSpace(EWidgetSpace::Screen);
}



void ACEnemy::BeginPlay()
{
	BodyMaterial = UMaterialInstanceDynamic::Create(Body, this);
	LogoMaterial = UMaterialInstanceDynamic::Create(Logo, this);

	GetMesh()->SetMaterial(0, BodyMaterial);
	GetMesh()->SetMaterial(1, LogoMaterial);

	State->OnStateTypeChanged.AddDynamic(this, &ACEnemy::OnStateTypeChanged);

	Super::BeginPlay();

	EnemyWidget->InitWidget();
	Cast<UCUserWidget_Enemy>(EnemyWidget->GetUserWidgetObject())->HealthUpdate(Status->GetHealth(), Status->GetMaxHealth());
	Cast<UCUserWidget_Enemy>(EnemyWidget->GetUserWidgetObject())->StunGauageUpdate(Status->GetCurrentStun(), Status->GetMaxStunLimit());
	Cast<UCUserWidget_Enemy>(EnemyWidget->GetUserWidgetObject())->SetVisibility(ESlateVisibility::Hidden);
	Cast<UCExecutionWidget>(ExecutionWidget->GetUserWidgetObject())->HideExecutionIcon();
	bDead = false;
}

void ACEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateAirBorne();
}



void ACEnemy::OnAction()
{
	if (Combat != nullptr)
		Combat->DoAction();
	else
		CLog::Log(GetName());
}

void ACEnemy::OnDashAction()
{
	if (State->IsIdleMode() == true)
		Combat->DoDashAction();
}

void ACEnemy::OnUpperAction()
{
	Combat->DoUpperAction();
}

void ACEnemy::OnTwoHand()
{
	Combat->SetTwoHandMode();
}

void ACEnemy::OnKnife()
{
	Combat->SetKnifeMode();
}

void ACEnemy::OnKatana()
{
	Combat->SetKatanaMode();
}

void ACEnemy::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
		case EStateType::Hitted: Hitted(); break;
		case EStateType::AirHitted: AirHitted(); break;
		case EStateType::Stun: Stunning(); break;
		case EStateType::Execution: Executed(); break;
		case EStateType::Dodge: Dodge(); break;
		case EStateType::Dead: Dead(); break;
	}
}

void ACEnemy::Launch_Character(FVector InDirection, float InPower)
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

int ACEnemy::SendDamage(FDoCombatData const& combatData, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (State->IsDeadMode())
		return -1;
	
	if (State->IsDodgeMode())
		return 1;

	if (State->IsExecutionMode())
		return -1;

	if (State->IsTeleport())
		return -1;

	TakeDamage(combatData.Power, DamageEvent, EventInstigator, DamageCauser);
	Status->AddStun(combatData.StunPower);
	Cast<UCUserWidget_Enemy>(EnemyWidget->GetUserWidgetObject())->StunGauageUpdate(Status->GetCurrentStun(), Status->GetMaxStunLimit());
	BeginStun();

	if (State->IsStunMode() == false && bDead == false)
	{
		DamageInstigator = EventInstigator;
		FVector start = GetActorLocation();
		FVector target = DamageInstigator->GetPawn()->GetActorLocation();
		FRotator rotator = UKismetMathLibrary::FindLookAtRotation(start, target);
		SetActorRotation(FQuat(FRotator(0, rotator.Yaw, 0)));

		FVector direction = target - start;
		direction.Normalize();

		bool bAirBorne = BeginAirBorne(combatData, direction);

		if (bAirBorne == false)
		{
			LaunchCharacter(-direction * combatData.PushPower, false, true);
			State->SetHittedMode();
		}
	}

	return 0;
}

void ACEnemy::Send_Freeze(const bool& InFreeze, const float& InDamage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	CheckNull(FreezeMontages);
	CheckTrue(State->IsDeadMode());
	bFreeze = InFreeze;

	if (InFreeze == true)
	{
		DamageInstigator = EventInstigator;
		FVector start = GetActorLocation();
		FVector target = DamageInstigator->GetPawn()->GetActorLocation();
		FRotator rotator = UKismetMathLibrary::FindLookAtRotation(start, target);
		SetActorRotation(FQuat(FRotator(0, rotator.Yaw, 0)));
		State->SetHittedMode();
		PlayAnimMontage(FreezeMontages, 1.0f, "Default");

		TakeDamage(InDamage, DamageEvent, EventInstigator, DamageCauser);

		return;
	}

	PlayAnimMontage(FreezeMontages, 1.0f, "FreezeRelease");
	DamageInstigator = NULL;
	
}

void ACEnemy::Hit_End()
{
	State->SetIdleMode();
	bSuperArmor = false;
}

void ACEnemy::End_Execution()
{
	Status->SubHealth(1000.0f);
	Cast<UCUserWidget_Enemy>(EnemyWidget->GetUserWidgetObject())->HealthUpdate(Status->GetHealth(), Status->GetMaxHealth());
	EnemyWidget->SetVisibility(false);
	UKismetSystemLibrary::K2_SetTimer(this, "ExecutionDead", 2.0f, false);
	if (OnDeadNotifier.IsBound())
	{
		OnDeadNotifier.Broadcast();
		OnDeadNotifier.Clear();
	}
	GetController()->UnPossess();
}

void ACEnemy::Set_SuperArmor(bool InSuperArmor)
{
	bSuperArmor = InSuperArmor;
}

void ACEnemy::BeginStun()
{
	CheckTrue(bFreeze);
	CheckTrue(State->IsDeadMode());
	CheckTrue(State->IsStunMode());
	CheckTrue(State->IsExecutionMode());

	bool b = true;
	b &= Status->IsStun();
	b &= (GetCharacterMovement()->IsFalling() == false);

	if (b && State->IsStunMode() == false)
	{
		State->SetStunMode();
		Cast<UCExecutionWidget>(ExecutionWidget->GetUserWidgetObject())->ShowExecutionIcon();
	}
}

void ACEnemy::EndStun()
{
	CheckTrue(State->IsDeadMode());
	CheckTrue(State->IsExecutionMode());

	Status->ResetStun();
	Cast<UCUserWidget_Enemy>(EnemyWidget->GetUserWidgetObject())->StunGauageUpdate(Status->GetCurrentStun(), Status->GetMaxStunLimit());
	Cast<UCExecutionWidget>(ExecutionWidget->GetUserWidgetObject())->HideExecutionIcon();
	State->SetIdleMode();
	StopAnimMontage();
}

void ACEnemy::ExecutionDead()
{
	if(Combat!=nullptr)
		Combat->End_Dead();
	
	Destroy();
}

float ACEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float damage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	Status->SubHealth(damage);

	Cast<UCUserWidget_Enemy>(EnemyWidget->GetUserWidgetObject())->HealthUpdate(Status->GetHealth(), Status->GetMaxHealth());

	if (Status->GetHealth() <= 0.0f)
	{
		State->SetDeadMode();
		bDead = true;
		return 0.0f;
	}

	return Status->GetHealth();
}

void ACEnemy::Hitted()
{
	CheckTrue(bSuperArmor);
	CheckTrue(bFreeze);
	Status->SetMove();
	Montages->PlayHitted();

	if (Combat != nullptr)
		Combat->AbortAction();
	
	HitCount++;
	DamageInstigator = NULL;
}

void ACEnemy::AirHitted()
{
	CheckTrue(bSuperArmor);
	StopAnimMontage();
	bAirHit = true;
	Status->SetMove();
	Montages->PlayAirBorne();

	DamageInstigator = NULL;

}

void ACEnemy::Dead()
{
	UKismetSystemLibrary::K2_ClearTimer(this,"EndStun");
	StopAnimMontage();
	EnemyWidget->SetVisibility(false);
	Montages->PlayDead();
	Cast<UCExecutionWidget>(ExecutionWidget->GetUserWidgetObject())->HideExecutionIcon();
	//if (OnDeadNotifier.IsBound())
	//{
	//	OnDeadNotifier.Broadcast();
	//	OnDeadNotifier.Clear();
	//}
	//GetController()->UnPossess();
}

void ACEnemy::Stunning()
{
	Montages->PlayStun();
	UKismetSystemLibrary::K2_SetTimer(this, "EndStun", StunTime, false);
}

void ACEnemy::Dodge()
{
	Montages->PlayDodge();
	HitCount = 0;
}

void ACEnemy::Executed()
{
	Cast<UCExecutionWidget>(ExecutionWidget->GetUserWidgetObject())->HideExecutionIcon();
}


void ACEnemy::End_Dead()
{
	if (OnDeadNotifier.IsBound())
	{
		OnDeadNotifier.Broadcast();
		OnDeadNotifier.Clear();
	}
	GetController()->UnPossess();

	if (Combat == nullptr)
		Combat = CHelpers::GetComponent<UPCombatComponent>(this);

	Combat->End_Dead();
	Destroy();
}

bool ACEnemy::BeginAirBorne(FDoCombatData const& IncombatData, FVector const& InDirection)
{
	if (bSuperArmor == true)
		return false;

	if (bFreeze == true)
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

void ACEnemy::UpdateAirBorne()
{
	if (State->IsDeadMode())
	{
		bAirHit = false;
		GetCharacterMovement()->GravityScale = 2.0f;
		return;
	}

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


