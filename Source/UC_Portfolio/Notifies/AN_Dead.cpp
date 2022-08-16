#include "Notifies/AN_Dead.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Characters/ICharacter.h"

FString UAN_Dead::GetNotifyName_Implementation() const
{
	return "Dead";
}

void UAN_Dead::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACharacter* ownerCharacter = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(ownerCharacter);

	IICharacter* iCharacter = Cast<IICharacter>(ownerCharacter);
	iCharacter->End_Dead();
}

