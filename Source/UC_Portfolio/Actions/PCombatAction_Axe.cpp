#include "Actions/PCombatAction_Axe.h"
#include "Global.h"
#include "Actions/PCombat.h"
#include "Actions/PArmed_Axe.h"
#include "Components/PCombatComponent.h"
#include "GameFramework/Character.h"


APCombatAction_Axe::APCombatAction_Axe()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APCombatAction_Axe::BeginPlay()
{
	Super::BeginPlay();
	

}

void APCombatAction_Axe::Tick(float DeltaTime)
{
	
}


void APCombatAction_Axe::DoAction()
{
	if ((*bAimed) == true)
	{
		DoAxeAction();
		return;
	}

	Super::DoAction();
	
}

void APCombatAction_Axe::Next_DoAction()
{
	Super::Next_DoAction();
}

void APCombatAction_Axe::End_DoAction()
{
	Super::End_DoAction();
}

void APCombatAction_Axe::OnAim()
{
	//*bAimed = true;
	UPCombatComponent* combat = CHelpers::GetComponent<UPCombatComponent>(OwnerCharacter);
	CheckNull(combat);
	APArmed_Axe* axe = Cast<APArmed_Axe>(combat->GetAxe());
	axe->AxeAim(*bAimed);
}

void APCombatAction_Axe::OffAim()
{
	//bAim = false;
	OwnerCharacter->StopAnimMontage();
	UPCombatComponent* combat = CHelpers::GetComponent<UPCombatComponent>(OwnerCharacter);
	CheckNull(combat);
	APArmed_Axe* axe = Cast<APArmed_Axe>(combat->GetAxe());
	axe->AxeAim(*bAimed);
}

void APCombatAction_Axe::DoDashAction()
{
	if ((*bAimed) == true)
	{
		DoAxeAction();
		return;
	}

	Super::DoDashAction();
}

void APCombatAction_Axe::DoUpperAction()
{
	if ((*bAimed) == true)
	{
		DoAxeAction();
		return;
	}

	Super::DoUpperAction();
}

void APCombatAction_Axe::DoSpecialAction()
{
	if ((*bAimed) == true)
	{
		DoAxeAction();
		return;
	}

	Super::DoSpecialAction();
}

void APCombatAction_Axe::DoSpecial2Action()
{
	if ((*bAimed) == true)
	{
		DoAxeAction();
		return;
	}

	Super::DoSpecial2Action();


}

void APCombatAction_Axe::OnBeginOverlap(AActor* InAttacker, AActor* InCauser, AActor* InOther, const bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnBeginOverlap(InAttacker, InCauser, InOther, bFromSweep, SweepResult);
}

void APCombatAction_Axe::OnEndOverlap(AActor* InAttacker, AActor* InAttackCauser, AActor* InOther)
{
	Super::OnEndOverlap(InAttacker, InAttackCauser, InOther);
}

void APCombatAction_Axe::DoAxeAction()
{
	UPCombatComponent* combat = CHelpers::GetComponent<UPCombatComponent>(OwnerCharacter);
	CheckNull(combat);
	APArmed_Axe* axe = Cast<APArmed_Axe>(combat->GetAxe());
	axe->AxeAction();
}

