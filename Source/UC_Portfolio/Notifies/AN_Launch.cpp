#include "Notifies/AN_Launch.h"
#include "Global.h"
#include "Characters/ICharacter.h"

FString UAN_Launch::GetNotifyName_Implementation() const
{
	return "Launch";
}

void UAN_Launch::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	IICharacter* character = Cast<IICharacter>(MeshComp->GetOwner());
	CheckNull(character);

	character->Launch_Character(LauchInfo.Direction, LauchInfo.LauchPower);

}