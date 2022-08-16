#include "Notifies/ANS_Collision.h"
#include "Global.h"
#include "Actions/PArmed.h"
#include "Actions/PCombat.h"
#include "Components/PCombatComponent.h"


FString UANS_Collision::GetNotifyName_Implementation() const
{
	return "Collisions";
}

void UANS_Collision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UPCombatComponent* combatAction = CHelpers::GetComponent<UPCombatComponent>(MeshComp->GetOwner());
	CheckNull(combatAction);

	APArmed* armed = combatAction->GetCurrent()->GetArmed();
	CheckNull(armed);

	if (CollisionName == "None")
	{
		//armed->OnCollision();
		armed->K2OnCollision();
		return;
	}

	//armed->OnCollision(CollisionName);
	armed->K2OnCollisionByName(CollisionName);
}

void UANS_Collision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UPCombatComponent* combatAction = CHelpers::GetComponent<UPCombatComponent>(MeshComp->GetOwner());
	CheckNull(combatAction);

	APArmed* armed = combatAction->GetCurrent()->GetArmed();
	CheckNull(armed);
	//armed->OffCollision();
	armed->K2OffCollision();

}