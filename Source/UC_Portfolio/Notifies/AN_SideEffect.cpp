#include "Notifies/AN_SideEffect.h"
#include "Global.h"
#include "Actions/PCombatAction.h"
#include "Components/PCombatComponent.h"
#include "Interface/IMelee.h"

FString UAN_SideEffect::GetNotifyName_Implementation() const
{
	return "SideEffect";
}

void UAN_SideEffect::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UPCombatComponent* combatAction = CHelpers::GetComponent<UPCombatComponent>(MeshComp->GetOwner());
	CheckNull(combatAction);

	IIMelee* melee = Cast<IIMelee>(combatAction->GetCurrent()->GetCombatAction());
	CheckNull(melee);
	melee->DoSideEffect();
}
