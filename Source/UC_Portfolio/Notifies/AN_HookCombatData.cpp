#include "Notifies/AN_HookCombatData.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Actions/PCombatAction.h"
#include "Components/PCombatComponent.h"

FString UAN_HookCombatData::GetNotifyName_Implementation() const
{
	return "HookCombatData";
}

void UAN_HookCombatData::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UPCombatComponent* combatAction = CHelpers::GetComponent<UPCombatComponent>(MeshComp->GetOwner());
	CheckNull(combatAction);

	combatAction->GetCurrent()->GetCombatAction()->HookCurrentCombatData
	(
		Power,
		HitStop,
		Angle,
		PushPower,
		AttackStyle
	);
}