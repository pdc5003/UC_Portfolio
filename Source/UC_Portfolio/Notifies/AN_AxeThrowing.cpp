#include "Notifies/AN_AxeThrowing.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Actions/PArmed_Axe.h"
#include "Components/PCombatComponent.h"

FString UAN_AxeThrowing::GetNotifyName_Implementation() const
{
	return "Throwing";
}

void UAN_AxeThrowing::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UPCombatComponent* combatAction = CHelpers::GetComponent<UPCombatComponent>(MeshComp->GetOwner());
	CheckNull(combatAction);
	
	APArmed_Axe* axe = Cast<APArmed_Axe>(combatAction->GetCurrent()->GetArmed());
	CheckNull(axe);

	axe->AN_Detach();
	
}