#include "Notifies/AN_AxeCatchEnd.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/PCombatComponent.h"
#include "Actions/PArmed_Axe.h"

FString UAN_AxeCatchEnd::GetNotifyName_Implementation() const
{
	return "CatchEnd";
}

void UAN_AxeCatchEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UPCombatComponent* combatAction = CHelpers::GetComponent<UPCombatComponent>(MeshComp->GetOwner());
	CheckNull(combatAction);

	APArmed_Axe* axe = Cast<APArmed_Axe>(combatAction->GetAxe());
	CheckNull(axe);

	axe->AN_CatchEnd();

}
