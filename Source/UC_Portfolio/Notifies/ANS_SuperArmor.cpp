#include "Notifies/ANS_SuperArmor.h"
#include "Global.h"
#include "Characters/ICharacter.h"

FString UANS_SuperArmor::GetNotifyName_Implementation() const
{
	return "SuperArmor";
}

void UANS_SuperArmor::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	IICharacter* character = Cast<IICharacter>(MeshComp->GetOwner());
	CheckNull(character);
	character->Set_SuperArmor(true);
}

void UANS_SuperArmor::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	IICharacter* character = Cast<IICharacter>(MeshComp->GetOwner());
	CheckNull(character);
	character->Set_SuperArmor(false);
}
