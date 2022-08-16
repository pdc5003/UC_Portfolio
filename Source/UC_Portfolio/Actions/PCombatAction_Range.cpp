#include "Actions/PCombatAction_Range.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"

APCombatAction_Range::APCombatAction_Range()
{

}

void APCombatAction_Range::BeginPlay()
{
	Super::BeginPlay();
}

void APCombatAction_Range::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APCombatAction_Range::DoAction()
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

	OwnerCharacter->PlayAnimMontage(DefaultCombatDatas[0].AnimMontage, DefaultCombatDatas[0].PlayRatio, DefaultCombatDatas[0].StartSection);
	return;

	//if ((*bAimed) == true)
	//{
	//	Super::DoAction();
	//
	//	CheckFalse(DefaultCombatDatas.Num() > 0);
	//
	//	if (bEnable == true)
	//	{
	//		bExist = true;
	//		bEnable = false;
	//
	//		return;
	//	}
	//
	//	CheckFalse(State->IsIdleMode());
	//	State->SetActionMode();
	//
	//	OwnerCharacter->PlayAnimMontage(DefaultCombatDatas[0].AnimMontage, DefaultCombatDatas[0].PlayRatio, DefaultCombatDatas[0].StartSection);
	//	return;
	//}

}

void APCombatAction_Range::Next_DoAction()
{
	Super::Next_DoAction();
	CheckFalse(bExist);
	bExist = false;

	OwnerCharacter->StopAnimMontage();
	Index++;

	OwnerCharacter->PlayAnimMontage(DefaultCombatDatas[Index].AnimMontage, DefaultCombatDatas[Index].PlayRatio, DefaultCombatDatas[Index].StartSection);
}

void APCombatAction_Range::End_DoAction()
{
	Super::End_DoAction();
	OwnerCharacter->StopAnimMontage(DefaultCombatDatas[Index].AnimMontage);

	Index = 0;

	State->SetIdleMode();
}

void APCombatAction_Range::OnAim()
{
}

void APCombatAction_Range::OffAim()
{
}
