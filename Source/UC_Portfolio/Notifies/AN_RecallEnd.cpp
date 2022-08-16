#include "Notifies/AN_RecallEnd.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Actions/PArmed_Axe.h"
#include "Components/PCombatComponent.h"

FString UAN_RecallEnd::GetNotifyName_Implementation() const
{
	return "RecallEnd";
}

void UAN_RecallEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UPCombatComponent* combatAction = CHelpers::GetComponent<UPCombatComponent>(MeshComp->GetOwner());
	CheckNull(combatAction);

	APArmed_Axe* axe = Cast<APArmed_Axe>(combatAction->GetAxe());
	CheckNull(axe);

	//axe->AN_RecallEnd();

}
