#include "Actions/PCombatAction_BareHand.h"
#include "Global.h"
#include "Actions/PCombat.h"
#include "Actions/PArmed_Axe.h"
#include "Components/PCombatComponent.h"
#include "GameFramework/Character.h"

APCombatAction_BareHand::APCombatAction_BareHand()
{

}

void APCombatAction_BareHand::BeginPlay()
{
	Super::BeginPlay();
}

void APCombatAction_BareHand::Tick(float DeltaTime)
{
}

void APCombatAction_BareHand::DoAction()
{
	if ((*bAimed) == true)
	{
		DoAxeAction();
		return;
	}

	Super::DoAction();
}

void APCombatAction_BareHand::Next_DoAction()
{
	Super::Next_DoAction();
}

void APCombatAction_BareHand::End_DoAction()
{
	Super::End_DoAction();
}

void APCombatAction_BareHand::OnAim()
{
	UPCombatComponent* combat = CHelpers::GetComponent<UPCombatComponent>(OwnerCharacter);
	CheckNull(combat);
	APArmed_Axe* axe = Cast<APArmed_Axe>(combat->GetAxe());
	axe->AxeAim(*bAimed);
}

void APCombatAction_BareHand::OffAim()
{
	OwnerCharacter->StopAnimMontage();
	UPCombatComponent* combat = CHelpers::GetComponent<UPCombatComponent>(OwnerCharacter);
	CheckNull(combat);
	APArmed_Axe* axe = Cast<APArmed_Axe>(combat->GetAxe());
	axe->AxeAim(*bAimed);
}

void APCombatAction_BareHand::DoDashAction()
{
	if ((*bAimed) == true)
	{
		DoAxeAction();
		return;
	}

	Super::DoDashAction();
}

void APCombatAction_BareHand::DoUpperAction()
{
	if ((*bAimed) == true)
	{
		DoAxeAction();
		return;
	}

	Super::DoUpperAction();
}

void APCombatAction_BareHand::DoSpecialAction()
{
	if ((*bAimed) == true)
	{
		DoAxeAction();
		return;
	}

	Super::DoSpecialAction();
}

void APCombatAction_BareHand::OnBeginOverlap(AActor* InAttacker, AActor* InCauser, AActor* InOther, const bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnBeginOverlap(InAttacker, InCauser, InOther, bFromSweep, SweepResult);
}

void APCombatAction_BareHand::OnEndOverlap(AActor* InAttacker, AActor* InAttackCauser, AActor* InOther)
{
	Super::OnEndOverlap(InAttacker, InAttackCauser, InOther);
}

void APCombatAction_BareHand::DoAxeAction()
{
	UPCombatComponent* combat = CHelpers::GetComponent<UPCombatComponent>(OwnerCharacter);
	CheckNull(combat);
	APArmed_Axe* axe = Cast<APArmed_Axe>(combat->GetAxe());
	axe->AxeAction();
}
