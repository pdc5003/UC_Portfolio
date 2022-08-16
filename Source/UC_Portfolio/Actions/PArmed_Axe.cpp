#include "Actions/PArmed_Axe.h"
#include "Global.h"
#include "Characters/PPlayer.h"
#include "Characters/ICharacter.h"
#include "Characters/CEnemy_AI_Boss.h"
#include "Components/MeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Components/PCombatComponent.h"
#include "Components/PFinisherComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"

APArmed_Axe::APArmed_Axe()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USceneComponent>(this, &SubRoot, "SubRoot", GetRootComponent());
	CHelpers::CreateComponent<USceneComponent>(this, &LodgePoint, "LodgePoint", SubRoot);

	CHelpers::CreateActorComponent<UProjectileMovementComponent>(this, &ProjectileMovement, "ProjectileMovement");
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	ProjectileMovement->bInitialVelocityInLocalSpace = false;
	ProjectileMovement->Velocity = FVector::ZeroVector;
	ProjectileMovement->bAutoActivate = false;
}

void APArmed_Axe::BeginPlay()
{
	Super::BeginPlay();
	Mesh = CHelpers::GetComponent<UMeshComponent>(this);

	OnThrowAxeUpdate.AddDynamic(this, &APArmed_Axe::UpdateAxeTrace);
	OnThrowAxeUpdate.AddDynamic(this, &APArmed_Axe::UpdateRotateAxeMesh);
	OnRecallAxeUpdate.AddDynamic(this, &APArmed_Axe::UpdateReturnAxe);

	UPCombatComponent* combat = CHelpers::GetComponent<UPCombatComponent>(OwnerCharacter);
	
	if (!!combat)
	{
		OnSetAxe.AddDynamic(combat, &UPCombatComponent::SetAxeMode);
		OnSetBareHand.AddDynamic(combat, &UPCombatComponent::SetBareHandMode);
	}

	UPFinisherComponent* finisher = CHelpers::GetComponent<UPFinisherComponent>(OwnerCharacter);
	CheckNull(finisher);
	finisher->OnQuickRecall.BindUFunction(this, "QuickRecall");
	
}


void APArmed_Axe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//Recalling Axe
	{
		if (IsAxeRecalling())
		{
			if (OnRecallAxeUpdate.IsBound())
				OnRecallAxeUpdate.Broadcast();

			return;
		}
	}

	//Throwing Axe
	{
		if (IsAxeThrowing())
		{
			if (OnThrowAxeUpdate.IsBound())
				OnThrowAxeUpdate.Broadcast();

			return;
		}
	}
	
}

void APArmed_Axe::AxeAction()
{
	if (IsAxeIdle())
	{
		AxeThrowing();
		return;
	}


	if (bCanRecall)
	{
		bCanRecall = false;
		Detach();
		AxeRecalling();
		return;
	}
}

void APArmed_Axe::AxeAim(const bool& bAim)
{
	bWasAim = bAim;
	if (bAim)
		PlayAnimMontage(EMontageKey::Aim);
}

void APArmed_Axe::QuickRecall()
{
	CheckTrue(GetParentActor() == OwnerCharacter);

	ShowAxeMesh();

	ProjectileMovement->Deactivate();
	StopThrowTrail();
	
	SetAxeIdle();
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	SubRoot->SetRelativeRotation(FQuat(FRotator::ZeroRotator));
	LodgePoint->SetRelativeRotation(FQuat(FRotator::ZeroRotator));
	Mesh->SetRelativeRotation(FQuat(FRotator::ZeroRotator));

	if (!!Target)
	{
		ACEnemy_AI_Boss* boss = Cast<ACEnemy_AI_Boss>(Target);

		if (IsValid(boss) == true)
			boss->UnRegistStuckArmed();

		IICharacter* iCharacter = Cast<IICharacter>(Target);
		if (!!iCharacter)
		{
			FDamageEvent e;
			iCharacter->Send_Freeze(false, 0.0f, e, OwnerCharacter->GetController(), this);
		}

		Target = NULL;
	}

	AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), "Hand_Axe");

	RecallTime = 0.0f;
	bCanRecall = false;

	if (OnSetAxe.IsBound())
		OnSetAxe.Broadcast();
}

void APArmed_Axe::AN_Detach()
{
	Detach();
	FVector axeLocation = GetActorLocation();
	FTransform cameraTransform = UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerCameraManager->GetTransform();
	FVector direction = FQuat(cameraTransform.GetRotation()).GetForwardVector();
	AxeEndPoint = cameraTransform.GetLocation() + (direction * TraceDistance);
	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), AxeEndPoint));
	float offset = (axeLocation - cameraTransform.GetLocation()).Size2D();

	SetActorLocation(cameraTransform.GetLocation() + direction * offset);

	FVector velocity = AxeThrowPower * direction;

	ProjectileMovement->Velocity = velocity;
	ProjectileMovement->Activate();

	if (OnSetBareHand.IsBound())
		OnSetBareHand.Broadcast();
}

void APArmed_Axe::AN_ThrowingEnd()
{
	if (bWasAim == true)
	{
		OwnerCharacter->StopAnimMontage();
		PlayAnimMontage(EMontageKey::Aim);
	}

	
}

void APArmed_Axe::AN_CatchEnd()
{
	if (bWasAim == true)
		PlayAnimMontage(EMontageKey::Aim);

}

void APArmed_Axe::Detach()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	
	PlayThrowTrail();
}

void APArmed_Axe::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == OwnerCharacter)
		return;

	if (IsAxeIdle())
		Super::OnComponentBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

}

void APArmed_Axe::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnComponentEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void APArmed_Axe::AxeThrowing()
{
	SetAxeThrowing();
	PlayAnimMontage(EMontageKey::Throw);
	OnCollision();
	//EnableRecall();
	UKismetSystemLibrary::K2_SetTimer(this, "EnableRecall", 0.5f, false);
}

void APArmed_Axe::EnableRecall()
{
	bCanRecall = true;
}

void APArmed_Axe::AxeRecalling()
{
	if (!!Target)
	{
		ACEnemy_AI_Boss* boss = Cast<ACEnemy_AI_Boss>(Target);
		if (IsValid(boss) == true)
			boss->UnRegistStuckArmed();

		IICharacter* iCharacter = Cast<IICharacter>(Target);
		if (!!iCharacter)
		{
			FDamageEvent e;
			iCharacter->Send_Freeze(false, 0.0f, e, OwnerCharacter->GetController(), this);
		}

		Target = NULL;
	}

	ShowAxeMesh();

	SetAxeRecalling();
	ProjectileMovement->Deactivate();
	LastAxeLocation = GetActorLocation();
	DistanceFromOwner = (OwnerCharacter->GetActorLocation() - LastAxeLocation).Size()*0.5f;
	DistanceFromOwner = UKismetMathLibrary::FClamp(DistanceFromOwner, 500.0f, MaxLimitDistance*0.6f);
	PlayThrowTrail();
	PlayAnimMontage(EMontageKey::Recall);
	
}

void APArmed_Axe::UpdateRotateAxeMesh()
{
	FRotator rotate = FRotator(AxeRotationSpeed * (-1.0f) * GetWorld()->GetDeltaSeconds(), 0.0f, 0.0f);
	Mesh->AddRelativeRotation(FQuat(rotate));
}

void APArmed_Axe::UpdateAxeTrace()
{
	FHitResult hitResult;

	FVector start = GetActorLocation();
	FVector end = GetVelocity().GetSafeNormal() * 50.0f + GetActorLocation();

	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	params.AddIgnoredActor(OwnerCharacter);

	GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_Visibility, params);

	if (hitResult.bBlockingHit)
	{
		SetAxeStick();
		HitLocation = hitResult.ImpactPoint;
		ImpactNormal = hitResult.ImpactNormal;
		BoneName = hitResult.BoneName;
		ProjectileMovement->Deactivate();
		StopThrowTrail();
		AxeIK();

		ACharacter* character = Cast<ACharacter>(hitResult.Actor);

		if (!!character)
		{
			//CheckBoss
			{
				ACEnemy_AI_Boss* boss = Cast<ACEnemy_AI_Boss>(character);

				if (IsValid(boss) == true)
				{
					boss->OnHideStuckArmed.AddDynamic(this, &APArmed_Axe::HideAxeMesh);
					boss->OnShowStuckArmed.AddDynamic(this, &APArmed_Axe::ShowAxeMesh);

					UCStateComponent* bossState = CHelpers::GetComponent<UCStateComponent>(boss);
					if (IsValid(bossState) == true)
					{
						if (bossState->IsTeleport() == false)
						{
							Target = character;
							AttachToComponent(character->GetMesh(), FAttachmentTransformRules::KeepWorldTransform, BoneName);
							IICharacter* iCharacter = Cast<IICharacter>(character);

							if (!!iCharacter)
							{
								FDamageEvent e;
								iCharacter->Send_Freeze(true, 7.0f, e, OwnerCharacter->GetController(), this);
							}
						}
					}
					return;
				}
				
			}//CheckBoss

			{
				Target = character;
				AttachToComponent(character->GetMesh(), FAttachmentTransformRules::KeepWorldTransform, BoneName);
				IICharacter* iCharacter = Cast<IICharacter>(character);

				if (!!iCharacter)
				{
					FDamageEvent e;
					iCharacter->Send_Freeze(true, 7.0f, e, OwnerCharacter->GetController(), this);
				}

				return;
			}
		}

		AttachToActor(hitResult.GetActor(), FAttachmentTransformRules::KeepWorldTransform);


		return;
	}

}

void APArmed_Axe::UpdateReturnAxe()
{
	if (RecallTime < 1.0f)
	{
		SetActorLocation(CalculateBezierCurve());
		FRotator rotate = FRotator(AxeReturnPitchSpeed * GetWorld()->GetDeltaSeconds(), 0.0f, 0.0f);
		Mesh->AddRelativeRotation(FQuat(rotate));

		if (SubRoot->GetRelativeRotation().Roll >= 50.0f)
			return;

		FRotator subRotate = FRotator(0.0f, 0.0f, (AxeReturnRollSpeed * GetWorld()->GetDeltaSeconds()));
		SubRoot->AddRelativeRotation(FQuat(subRotate));
	}
	else
	{
		SetAxeIdle();
		SubRoot->SetRelativeRotation(FQuat(FRotator::ZeroRotator));
		LodgePoint->SetRelativeRotation(FQuat(FRotator::ZeroRotator));
		Mesh->SetRelativeRotation(FQuat(FRotator::ZeroRotator));

		AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), "Hand_Axe");

		RecallTime = 0.0f;
		bCanRecall = false;


		PlayCatchParticle();
		StopThrowTrail();
				
		PlayAnimMontage(EMontageKey::Cacth);
		
		if (!!CamShake)
			UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerCameraManager->StartCameraShake(CamShake);

		if (OnSetAxe.IsBound())
			OnSetAxe.Broadcast();

	}

}
void APArmed_Axe::PlayAnimMontage(EMontageKey InKey)
{
	OwnerCharacter->PlayAnimMontage
	(
		ThrowMontages[InKey].AnimMontage,
		ThrowMontages[InKey].PlayRatio,
		ThrowMontages[InKey].StartSection
	);
}

void APArmed_Axe::AxeIK()
{
	Mesh->SetRelativeRotation(FQuat(FRotator(0.0f, 0.0f, 0.0f)));
	LodgePoint->SetRelativeRotation(AdjustAxeImpactRotation());
	SetActorLocation(AdjustAxeImpactLocation());
}
/// <summary>
/// anchor : 거리에 따른 중간점 보간 값
/// p1 : 회수 시작시 도끼 위치값과 중간점 보간 값
/// p2 : 중간점과 endPoint 보간 값
/// result : p1과 p2 보간 값
/// </summary>
/// <returns></returns>
FVector APArmed_Axe::CalculateBezierCurve()
{
	FVector endPoint = OwnerCharacter->GetMesh()->GetSocketLocation("Hand_Axe");

	float dist = (GetActorLocation() - endPoint).Size();
	dist = UKismetMathLibrary::FClamp(dist, 1000.0f, MaxLimitDistance);
	
	RecallTime += GetWorld()->GetDeltaSeconds()*(AxeAceelSpeed * (1000.0f / dist));

	FVector side = FQuat(OwnerCharacter->GetActorRotation()).GetRightVector();
	FVector anchor = UKismetMathLibrary::VLerp(endPoint + (side * DistanceFromOwner),endPoint,RecallTime);
	FVector p1 = UKismetMathLibrary::VLerp(LastAxeLocation, anchor, RecallTime);
	FVector p2 = UKismetMathLibrary::VLerp(anchor, endPoint, RecallTime);
	FVector result = UKismetMathLibrary::VLerp(p1, p2, RecallTime);
	
	return result;
}

/// <summary>
/// 도끼날과 현재 부딪힌 위치 값을 이용해
/// 도끼와 부딪힌 물체사이의 거리 값을 보정해 주는 함수
/// </summary>
/// <returns></returns>
FVector APArmed_Axe::AdjustAxeImpactLocation()
{
	float pitch = ImpactNormal.Rotation().Pitch;
	float inversePitchCos = 1.0f - UKismetMathLibrary::DegCos(pitch);

	if (UKismetMathLibrary::NearlyEqual_FloatFloat(inversePitchCos, 0.0f, 0.1f))
		inversePitchCos = 0.1f;

	float intervalZ = 1.0f / inversePitchCos;
	FVector intervalLocation = HitLocation + FVector(0.0f, 0.0f, intervalZ * 0.85f);

	return (intervalLocation + (GetActorLocation() - LodgePoint->GetComponentLocation()));
}

/// <summary>
/// 도끼날이 부딪힌 위치에 자연스럽게 꽂히기 위해
/// 각도를 보정해 주는 함수
/// </summary>
/// <returns></returns>
FRotator APArmed_Axe::AdjustAxeImpactRotation()
{
	FRotator impactRotate = FRotator::ZeroRotator;

	float normalPitch = ImpactNormal.Rotation().Pitch;
	float errorPitch = normalPitch > 0.0f ? -UKismetMathLibrary::RandomFloatInRange(5.0f, 25.0f) : -UKismetMathLibrary::RandomFloatInRange(35.0f, 55.0f);
	float calculatePitch = errorPitch - normalPitch;
	impactRotate.Roll = UKismetMathLibrary::RandomFloatInRange(-3.0f, 3.0f);
	impactRotate.Pitch = calculatePitch;

	return impactRotate;
}

void APArmed_Axe::PlayThrowTrail()
{
	if (ThrowParticleComponent == nullptr)
	{
		ThrowParticleComponent = UGameplayStatics::SpawnEmitterAttached
		(
			ThrowParticle,
			Mesh,
			"",
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::SnapToTarget,
			false,
			EPSCPoolMethod::None,
			true
		);
	}
	ThrowParticleComponent->EndTrails();
	ThrowParticleComponent->BeginTrails("BaseSocket", "TipSocket", ETrailWidthMode::ETrailWidthMode_FromCentre, 1.0f);

}

void APArmed_Axe::StopThrowTrail()
{
	if (!!ThrowParticleComponent)
		ThrowParticleComponent->EndTrails();
}

void APArmed_Axe::PlayCatchParticle()
{
	if (CatchParticleComponent == nullptr)
	{
		CatchParticleComponent = UGameplayStatics::SpawnEmitterAttached
		(
			CatchParticle,
			Mesh,
			"",
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::SnapToTarget,
			false,
			EPSCPoolMethod::None,
			false
		);

		CatchParticleComponent->SetActorParameter("VertSurfaceActor", this);
	}

	CatchParticleComponent->Activate();
}

void APArmed_Axe::HideAxeMesh()
{
	Mesh->SetVisibility(false);
}

void APArmed_Axe::ShowAxeMesh()
{
	if (Mesh->IsVisible())
		return;

	Mesh->SetVisibility(true);
}

void APArmed_Axe::SetAxeIdle()
{
	SetAxeState(EAxeState::Idle);
}

void APArmed_Axe::SetAxeThrowing()
{
	SetAxeState(EAxeState::Throwing);
}

void APArmed_Axe::SetAxeStick()
{
	SetAxeState(EAxeState::Stick);
}

void APArmed_Axe::SetAxeRecalling()
{
	SetAxeState(EAxeState::Recalling);
}

void APArmed_Axe::SetAxeState(EAxeState InNewState)
{
	EAxeState prevState = AxeState;
	AxeState = InNewState;
}
