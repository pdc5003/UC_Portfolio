#include "Notifies/AN_EndExecution.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Characters/ICharacter.h"

FString UAN_EndExecution::GetNotifyName_Implementation() const
{
	return "EndExecution";
}

void UAN_EndExecution::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACharacter* ownerCharacter = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(ownerCharacter);

	IICharacter* iCharacter = Cast<IICharacter>(ownerCharacter);
	iCharacter->End_Execution();
	
}