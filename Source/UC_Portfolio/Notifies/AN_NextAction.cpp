#include "Notifies/AN_NextAction.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Actions/PCombatAction.h"
#include "Components/PCombatComponent.h"

FString UAN_NextAction::GetNotifyName_Implementation() const
{
	return "NextAction";
}

void UAN_NextAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UPCombatComponent* combatAction = CHelpers::GetComponent<UPCombatComponent>(MeshComp->GetOwner());
	CheckNull(combatAction);

	combatAction->GetCurrent()->GetCombatAction()->Next_DoAction();
}
