#include "Actions/PCombatAction_Katana.h"
#include "Global.h"
#include "Actions/PCombat.h"
#include "Actions/PArmed_Axe.h"
#include "Components/PCombatComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "GameFramework/Character.h"


APCombatAction_Katana::APCombatAction_Katana()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APCombatAction_Katana::BeginPlay()
{
	Super::BeginPlay();
	

}

void APCombatAction_Katana::Tick(float DeltaTime)
{
	
}


void APCombatAction_Katana::DoAction()
{
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
	OnDefaultAtkFlag();
	UKismetMathLibrary::RandomIntegerInRange(0, 1000) > 550 ? Index = 0  : Index = 3;

	OwnerCharacter->PlayAnimMontage(CurCombatDatas[Index].AnimMontage, CurCombatDatas[Index].PlayRatio, CurCombatDatas[Index].StartSection);
	CurCombatDatas[Index].bCanMove ? Status->SetMove() : Status->SetStop();
}

void APCombatAction_Katana::Next_DoAction()
{
	if (IsOnUpperAtkFlag())
	{
		if (SendDamageSuccess == 0)
			Super::Next_DoAction();

		return;
	}

	Super::Next_DoAction();
}

void APCombatAction_Katana::End_DoAction()
{
	Super::End_DoAction();
	AtkFlag = 0U;
	bHit = false;
}

void APCombatAction_Katana::AbortDoAction()
{
	Super::AbortDoAction();
	AtkFlag = 0U;
	bHit = false;
}


void APCombatAction_Katana::DoDashAction()
{
	Super::DoDashAction();
	OnDashAtkFlag();
}

void APCombatAction_Katana::DoUpperAction()
{
	Super::DoUpperAction();
	OnUpperAtkFlag();
}

void APCombatAction_Katana::DoSpecialAction()
{
	Super::DoSpecialAction();
}

void APCombatAction_Katana::DoSpecial2Action()
{
	Super::DoSpecial2Action();
}

void APCombatAction_Katana::OnBeginOverlap(AActor* InAttacker, AActor* InCauser, AActor* InOther, const bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnBeginOverlap(InAttacker, InCauser, InOther, bFromSweep, SweepResult);
}

void APCombatAction_Katana::OnEndOverlap(AActor* InAttacker, AActor* InAttackCauser, AActor* InOther)
{
	Super::OnEndOverlap(InAttacker, InAttackCauser, InOther);
}


