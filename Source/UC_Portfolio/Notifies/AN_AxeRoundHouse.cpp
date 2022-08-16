#include "Notifies/AN_AxeRoundHouse.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/PCombatComponent.h"
#include "Actions/PArmed_Axe.h"

FString UAN_AxeRoundHouse::GetNotifyName_Implementation() const
{
	return "RoundHouse";
}

void UAN_AxeRoundHouse::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UPCombatComponent* combatAction = CHelpers::GetComponent<UPCombatComponent>(MeshComp->GetOwner());
	CheckNull(combatAction);

	APArmed_Axe* axe = Cast<APArmed_Axe>(combatAction->GetAxe());
	CheckNull(axe);

}
