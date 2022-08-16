#include "Notifies/AN_EndAction.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Actions/PCombatAction.h"
#include "Components/PCombatComponent.h"

FString UAN_EndAction::GetNotifyName_Implementation() const
{
	return "EndAction";
}

void UAN_EndAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UPCombatComponent* combatAction = CHelpers::GetComponent<UPCombatComponent>(MeshComp->GetOwner());

	if (combatAction != NULL)
	{
		combatAction->GetCurrent()->GetCombatAction()->End_DoAction();
		return;
	}
		
}
