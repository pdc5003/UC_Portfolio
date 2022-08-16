#include "Characters/CEnemy_AI_Boss.h"
#include "Global.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/PCombatComponent.h"
#include "Components/SceneComponent.h"
#include "Actors/CThrowingWeapon.h"


ACEnemy_AI_Boss::ACEnemy_AI_Boss()
{
	CHelpers::CreateComponent<USceneComponent>(this, &SpPoint01, "SpPoint01", GetCapsuleComponent());
	CHelpers::CreateComponent<USceneComponent>(this, &SpPoint02, "SpPoint02", GetCapsuleComponent());
	CHelpers::CreateComponent<USceneComponent>(this, &SpPoint03, "SpPoint03", GetCapsuleComponent());

}

void ACEnemy_AI_Boss::BeginPlay()
{
	Super::BeginPlay();
	State->OnStateTypeChanged.AddDynamic(this, &ACEnemy_AI_Boss::OnBossStateTypeChanged);
}

void ACEnemy_AI_Boss::OnTeleport()
{
	State->SetTeleportMode();
	Montages->PlayTeleport();
}

void ACEnemy_AI_Boss::OffTeleport()
{
	StopAnimMontage();
	Montages->PlayTeleport("Appear");
	State->SetIdleMode();
}

void ACEnemy_AI_Boss::Begin_Teleport()
{
	USkeletalMeshComponent* skeletalComp = GetMesh();
	CheckNull(skeletalComp);
	skeletalComp->SetVisibility(false);

	if (!!Combat)
		Combat->SetHideWeapon(true);

}

void ACEnemy_AI_Boss::End_Teleport()
{
	USkeletalMeshComponent* skeletalComp = GetMesh();
	CheckNull(skeletalComp);
	skeletalComp->SetVisibility(true);

	
	if (!!Combat)
		Combat->SetHideWeapon(false);
}

void ACEnemy_AI_Boss::Send_Freeze(const bool& InFreeze, const float& InDamage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	CheckFalse(InFreeze);
	CheckNull(FreezeMontages);
	CheckTrue(State->IsDeadMode());

	State->SetHittedMode();
	StopAnimMontage();
	PlayAnimMontage(FreezeMontages);
	TakeDamage(InDamage, DamageEvent, EventInstigator, DamageCauser);

	if (IsValid(EventInstigator) == true)
	{
		FVector start = GetActorLocation();
		FVector target = EventInstigator->GetPawn()->GetActorLocation();
		FRotator rotator = UKismetMathLibrary::FindLookAtRotation(start, target);
		SetActorRotation(FQuat(FRotator(0, rotator.Yaw, 0)));
	}
	}

void ACEnemy_AI_Boss::SpawnSummonSword()
{
	CheckNull(SummonSword);

	FVector location = FVector::ZeroVector;
	GetRandomLocation(location);

	FTransform transform;
	transform.SetLocation(location);

	FRotator rotate = this->GetActorRotation();
	transform.SetRotation(FQuat(rotate));

	transform.SetScale3D(FVector::OneVector);

	ACThrowingWeapon* summon = GetWorld()->SpawnActorDeferred<ACThrowingWeapon>
	(
		SummonSword,
		transform,
		this,
		NULL,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);
	summon->Initialize();
	UGameplayStatics::FinishSpawningActor(summon, transform);
}

void ACEnemy_AI_Boss::GetRandomLocation(FVector& InLocation)
{
	int32 locationNum = UKismetMathLibrary::RandomIntegerInRange(0, 1000);

	if (locationNum > 500)
	{
		InLocation = UKismetMathLibrary::VLerp(SpPoint01->GetComponentLocation(), SpPoint02->GetComponentLocation(), UKismetMathLibrary::RandomFloatInRange(0, 1));
		return;
	}

	InLocation = UKismetMathLibrary::VLerp(SpPoint02->GetComponentLocation(), SpPoint03->GetComponentLocation(), UKismetMathLibrary::RandomFloatInRange(0,1));
	return;
	
}

void ACEnemy_AI_Boss::UnRegistStuckArmed()
{
	OnHideStuckArmed.Clear();
	OnShowStuckArmed.Clear();
}

void ACEnemy_AI_Boss::OnBossStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
		case EStateType::Idle: 
			{
				if (OnShowStuckArmed.IsBound())
					OnShowStuckArmed.Broadcast();
			}
			break;
		case EStateType::Teleport: 
			{
				if (OnHideStuckArmed.IsBound())
					OnHideStuckArmed.Broadcast();
			}
			break;
	}
}
