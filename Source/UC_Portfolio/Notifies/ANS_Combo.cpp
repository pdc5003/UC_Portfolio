#include "Notifies/ANS_Combo.h"
#include "Global.h"
#include "Actions/PCombatAction_Melee.h"
#include "Components/PCombatComponent.h"

FString UANS_Combo::GetNotifyName_Implementation() const
{
	return "Combo";
}

void UANS_Combo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UPCombatComponent* combatAction = CHelpers::GetComponent<UPCombatComponent>(MeshComp->GetOwner());
	CheckNull(combatAction);

	APCombatAction_Melee* combatMelee = Cast<APCombatAction_Melee>(combatAction->GetCurrent()->GetCombatAction());
	if (!!combatMelee)
	{
		combatMelee->EnableCombo();
		return;
	}		
}

void UANS_Combo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UPCombatComponent* combatAction = CHelpers::GetComponent<UPCombatComponent>(MeshComp->GetOwner());
	CheckNull(combatAction);

	APCombatAction_Melee* combatMelee = Cast<APCombatAction_Melee>(combatAction->GetCurrent()->GetCombatAction());
	if (!!combatMelee)
		combatMelee->DisableCombo();


}