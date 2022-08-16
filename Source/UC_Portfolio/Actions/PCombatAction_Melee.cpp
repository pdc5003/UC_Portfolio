#include "Actions/PCombatAction_Melee.h"
#include "Global.h"
#include "Actions/CSkillSideEffect.h"
#include "GameFramework/Character.h"
#include "Characters/ICharacter.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/PCombatComponent.h"
#include "GenericTeamAgentInterface.h"
#include "Particles/ParticleSystem.h"

APCombatAction_Melee::APCombatAction_Melee()
{

}

void APCombatAction_Melee::BeginPlay()
{
	Super::BeginPlay();
}

void APCombatAction_Melee::DoAction()
{
	Super::DoAction();

	CheckFalse(DefaultCombatDatas.Num() > 0);


	if (bEnable == true)
	{
		bExist = true;
		bEnable = false;

		return;
	}

	CheckFalse(State->IsIdleMode());
	State->SetActionMode();

	CurCombatDatas = DefaultCombatDatas;

	OwnerCharacter->PlayAnimMontage(CurCombatDatas[0].AnimMontage, CurCombatDatas[0].PlayRatio, CurCombatDatas[0].StartSection);
	CurCombatDatas[0].bCanMove ? Status->SetMove() : Status->SetStop();

}

void APCombatAction_Melee::DoDashAction()
{
	CheckFalse(SkillCombatDatas.Num() > 0);
	CheckFalse(State->IsIdleMode());
	State->SetActionMode();

	CurCombatDatas = SkillCombatDatas;

	OwnerCharacter->StopAnimMontage();
	OwnerCharacter->PlayAnimMontage(CurCombatDatas[0].AnimMontage, CurCombatDatas[0].PlayRatio, CurCombatDatas[0].StartSection);
	Index = 0;
	CurCombatDatas[Index].bCanMove ? Status->SetMove() : Status->SetStop();

}

void APCombatAction_Melee::DoUpperAction()
{
	CheckFalse(SkillCombatDatas.Num() > 0);
	CheckFalse(State->IsIdleMode());
	State->SetActionMode();

	CurCombatDatas = SkillCombatDatas;

	OwnerCharacter->StopAnimMontage();
	OwnerCharacter->PlayAnimMontage(CurCombatDatas[1].AnimMontage, CurCombatDatas[1].PlayRatio, CurCombatDatas[1].StartSection);
	Index = 1;
	CurCombatDatas[Index].bCanMove ? Status->SetMove() : Status->SetStop();
}

void APCombatAction_Melee::DoSpecialAction()
{
	CheckFalse(SkillCombatDatas.Num() > 0);
	CheckFalse(State->IsIdleMode());
	State->SetActionMode();

	CurCombatDatas = SkillCombatDatas;

	OwnerCharacter->StopAnimMontage();
	OwnerCharacter->PlayAnimMontage(CurCombatDatas[2].AnimMontage, CurCombatDatas[2].PlayRatio, CurCombatDatas[2].StartSection);
	Index = 2;
	CurCombatDatas[Index].bCanMove ? Status->SetMove() : Status->SetStop();
}

void APCombatAction_Melee::DoSpecial2Action()
{
	//CheckFalse(SkillCombatDatas.Num() > 0);
	//CheckFalse(State->IsIdleMode());
	//State->SetActionMode();

	//CurCombatDatas = SkillCombatDatas;

	//OwnerCharacter->StopAnimMontage();
	//OwnerCharacter->PlayAnimMontage(CurCombatDatas[3].AnimMontage, CurCombatDatas[3].PlayRatio, CurCombatDatas[3].StartSection);
	//Index = 3;
	//CurCombatDatas[Index].bCanMove ? Status->SetMove() : Status->SetStop();
}

void APCombatAction_Melee::DoSideEffect()
{
	CheckFalse(CurCombatDatas.IsValidIndex(Index));
	CheckNull(CurCombatDatas[Index].SideEffectClass);

	FTransform transform;
	transform.SetLocation(OwnerCharacter->GetActorLocation());
	transform.SetRotation(FQuat(OwnerCharacter->GetActorRotation()));
	transform.SetScale3D(FVector::OneVector);

	FActorSpawnParameters params;
	params.Owner = OwnerCharacter;

	ACSkillSideEffect* sideEffect = GetWorld()->SpawnActorDeferred<ACSkillSideEffect>
		(
			CurCombatDatas[Index].SideEffectClass,
			transform,
			OwnerCharacter,
			NULL,
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);
	
	UGameplayStatics::FinishSpawningActor(sideEffect, transform);
}


void APCombatAction_Melee::Next_DoAction()
{
	Super::Next_DoAction();
	CheckFalse(bExist);
	bExist = false;

	OwnerCharacter->StopAnimMontage();
	Index++;

	Index = Index % CurCombatDatas.Num();

	OwnerCharacter->PlayAnimMontage(CurCombatDatas[Index].AnimMontage, CurCombatDatas[Index].PlayRatio, CurCombatDatas[Index].StartSection);
	CurCombatDatas[Index].bCanMove ? Status->SetMove() : Status->SetStop();

}

void APCombatAction_Melee::End_DoAction()
{
	Super::End_DoAction();
	OwnerCharacter->StopAnimMontage();
	Index = 0;
	SendDamageSuccess = -1;
	State->SetIdleMode();
	Status->SetMove();

	CurCombatDatas.Empty();
	HittedCharacter.Empty();
}

void APCombatAction_Melee::AbortDoAction()
{
	Super::AbortDoAction();
	Index = 0;
	SendDamageSuccess = -1;
	CurCombatDatas.Empty();
	HittedCharacter.Empty();
}

void APCombatAction_Melee::HookCurrentCombatData(const float& Power, const float& HitStop, const float& Angle, const float& PushPower, int32 AttackStyle)
{
	Super::HookCurrentCombatData(Power, HitStop, Angle, PushPower, AttackStyle);
	CurCombatDatas[Index].Power = Power;
	CurCombatDatas[Index].HitStop = HitStop;
	CurCombatDatas[Index].Angle = Angle;
	CurCombatDatas[Index].PushPower = PushPower;
	CurCombatDatas[Index].AttackStyle = (EAttackStyle)AttackStyle;
}

void APCombatAction_Melee::OnBeginOverlap(AActor* InAttacker, AActor* InCauser, AActor* InOther, const bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnBeginOverlap(InAttacker, InCauser, InOther, bFromSweep, SweepResult);

	ACharacter* otherCharacter = Cast<ACharacter>(InOther);

	if (!!otherCharacter)
	{
		for (const ACharacter* other : HittedCharacter)
		{
			if (otherCharacter == other)
				return;
		}

		IGenericTeamAgentInterface* otherAgent = Cast<IGenericTeamAgentInterface>(otherCharacter);
		IGenericTeamAgentInterface* ownerAgent = Cast<IGenericTeamAgentInterface>(OwnerCharacter);

		CheckNull(otherAgent);
		CheckNull(ownerAgent);

		if (otherAgent->GetGenericTeamId() == ownerAgent->GetGenericTeamId())
			return;

		HittedCharacter.AddUnique(otherCharacter);

		CheckFalse(CurCombatDatas.IsValidIndex(Index));

		FDamageEvent e;
		ACharacter* attacker = Cast<ACharacter>(InAttacker);
		IICharacter* other = Cast<IICharacter>(otherCharacter);

		//int sendDamageSuccess = -1;

		if (!!other)
			SendDamageSuccess = other->SendDamage(CurCombatDatas[Index], e, attacker->GetController(), InCauser);

		if (SendDamageSuccess == 0)
		{
			float hitStop = CurCombatDatas[Index].HitStop;
			if (FMath::IsNearlyZero(hitStop) == false)
			{
				UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1e-3f);
				UKismetSystemLibrary::K2_SetTimer(this, "ReturnToGlobalDilation", hitStop * 1e-3f, false);
			}

			TSubclassOf<UMatineeCameraShake> shake = CurCombatDatas[Index].ShakeClass;
			if (!!shake)
				UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerCameraManager->StartCameraShake(shake);

			UParticleSystem* particle = CurCombatDatas[Index].Effect;
			if (!!particle)
			{
				FTransform particleTransform = CurCombatDatas[Index].ParticleTransform;
				particleTransform.AddToTranslation(SweepResult.Location);
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), particle, particleTransform);
			}

			UNiagaraSystem* niagara = CurCombatDatas[Index].NiagaraEffect;
			if (!!niagara)
			{
				FTransform niagaraTransform = CurCombatDatas[Index].NiagaraTransform;
				niagaraTransform.AddToTranslation(SweepResult.Location);
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), niagara, niagaraTransform.GetLocation(), niagaraTransform.GetRotation().Rotator(), niagaraTransform.GetScale3D());
			}

			return;
		}
		
		if (SendDamageSuccess == 1)
		{
			UPCombatComponent* combat = CHelpers::GetComponent<UPCombatComponent>(otherCharacter);
			if (!!combat)
			{
				bool bPerfect = combat->IsPerfectBlocking();
				
				FTransform particleTransform;
				FVector dir = (SweepResult.Location - OwnerCharacter->GetActorLocation()).GetSafeNormal();
				FVector location = SweepResult.Location + (-dir * BlockParticleOffset);

				if (bPerfect)
				{
					if (!!PerfectBlockParticle)
					{
						particleTransform.AddToTranslation(location);
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PerfectBlockParticle, particleTransform);
					}
				}
				else
				{
					if (!!NormalBlockParticle)
					{
						particleTransform.AddToTranslation(location);
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), NormalBlockParticle, particleTransform);
					}
				}
			}
			
			return;
		}

	}
}

void APCombatAction_Melee::OnEndOverlap(AActor* InAttacker, AActor* InAttackCauser, AActor* InOther)
{
	Super::OnEndOverlap(InAttacker, InAttackCauser, InOther);
	HittedCharacter.Empty();
}

void APCombatAction_Melee::ReturnToGlobalDilation()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
}



