#include "Notifies/ANS_Teleport.h"
#include "Global.h"
#include "Characters/CEnemy_AI_Boss.h"

FString UANS_Teleport::GetNotifyName_Implementation() const
{
	return "Teleport";
}

void UANS_Teleport::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACEnemy_AI_Boss* character = Cast<ACEnemy_AI_Boss>(MeshComp->GetOwner());
	CheckNull(character);
	character->Begin_Teleport();
}

void UANS_Teleport::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACEnemy_AI_Boss* character = Cast<ACEnemy_AI_Boss>(MeshComp->GetOwner());
	CheckNull(character);
	character->End_Teleport();
}
