#include "Actions/PCombatAction_Shield.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Characters/ICharacter.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/PCombatComponent.h"

APCombatAction_Shield::APCombatAction_Shield()
{
	
}

void APCombatAction_Shield::BeginPlay()
{
	Super::BeginPlay();
	bBlockingTimingCheck = false;
	bPerfectBlocking = false;
	
	UPCombatComponent* combat = CHelpers::GetComponent<UPCombatComponent>(OwnerCharacter);
	if (!!combat)
		combat->SetBlocking(&bPerfectBlocking);
}

void APCombatAction_Shield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckFalse(bBlockingTimingCheck);

	float timimg = UKismetSystemLibrary::GetGameTimeInSeconds(GetWorld());
	(timimg - BlockStartTime) < CounterAttackAbleTime ? bPerfectBlocking = true : bPerfectBlocking = false;
}

void APCombatAction_Shield::DoAction()
{
	CheckFalse(DefaultCombatDatas.Num() > 0);

	CheckFalse(State->IsIdleMode());
	CheckTrue(State->IsActionMode());
	State->SetBlockMode();

	CurCombatDatas = DefaultCombatDatas;
	bCounterAttack = true;
	OwnerCharacter->PlayAnimMontage(CurCombatDatas[0].AnimMontage, CurCombatDatas[0].PlayRatio, CurCombatDatas[0].StartSection);
	CurCombatDatas[0].bCanMove ? Status->SetMove() : Status->SetStop();

	BlockStartTime = UKismetSystemLibrary::GetGameTimeInSeconds(GetWorld());
	bBlockingTimingCheck = true;
	
}

void APCombatAction_Shield::End_DoAction()
{
	DoActionCancle();
	Super::End_DoAction();

	UPCombatComponent* combat = CHelpers::GetComponent<UPCombatComponent>(OwnerCharacter);
	if (!!combat)
		combat->GetBackCombatType();

	HittedCharacter.Empty();

	bBlockingTimingCheck = false;
}

void APCombatAction_Shield::AbortDoAction()
{
	Super::AbortDoAction();

	UPCombatComponent* combat = CHelpers::GetComponent<UPCombatComponent>(OwnerCharacter);
	if (!!combat)
		combat->GetBackCombatType();

	HittedCharacter.Empty();

	bBlockingTimingCheck = false;
}

void APCombatAction_Shield::DoActionCancle()
{
	OwnerCharacter->PlayAnimMontage(CurCombatDatas[1].AnimMontage, CurCombatDatas[1].PlayRatio, CurCombatDatas[1].StartSection);
	CurCombatDatas[1].bCanMove ? Status->SetMove() : Status->SetStop();

}

void APCombatAction_Shield::DoCounterAction()
{
	if (bPerfectBlocking == true && bCounterAttack == true)
	{
		State->SetActionMode();
		bCounterAttack = false;
		bBlockingTimingCheck = false;
		Index = 2;
		OwnerCharacter->PlayAnimMontage(CurCombatDatas[Index].AnimMontage, CurCombatDatas[Index].PlayRatio, CurCombatDatas[Index].StartSection);
		CurCombatDatas[Index].bCanMove ? Status->SetMove() : Status->SetStop();
	}
}


void APCombatAction_Shield::OnBeginOverlap(AActor* InAttacker, AActor* InCauser, AActor* InOther, const bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnBeginOverlap(InAttacker, InCauser, InOther, bFromSweep, SweepResult);
}

void APCombatAction_Shield::OnEndOverlap(AActor* InAttacker, AActor* InAttackCauser, AActor* InOther)
{
	//
}