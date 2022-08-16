#include "Notifies/AN_HitEnd.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Characters/ICharacter.h"

FString UAN_HitEnd::GetNotifyName_Implementation() const
{
	return "HitEnd";
}

void UAN_HitEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());
	
	ACharacter* ownerCharacter = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(ownerCharacter);
	
	IICharacter* iCharacter = Cast<IICharacter>(ownerCharacter);
	iCharacter->Hit_End();
}
