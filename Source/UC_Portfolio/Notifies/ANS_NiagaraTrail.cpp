#include "Notifies/ANS_NiagaraTrail.h"
#include "Global.h"
#include "Components/PCombatComponent.h"

FString UANS_NiagaraTrail::GetNotifyName_Implementation() const
{
	return "NiagaraTrail";
}

void UANS_NiagaraTrail::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UPCombatComponent* combatAction = CHelpers::GetComponent<UPCombatComponent>(MeshComp->GetOwner());
	CheckNull(combatAction);
	combatAction->ActiveNiagaraTrail();
}

void UANS_NiagaraTrail::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UPCombatComponent* combatAction = CHelpers::GetComponent<UPCombatComponent>(MeshComp->GetOwner());
	CheckNull(combatAction);
	combatAction->DeActiveNiagaraTrail();
}
