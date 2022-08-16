#include "Notifies/AN_MontagePause.h"
#include "Global.h"
#include "GameFramework/Character.h"

FString UAN_MontagePause::GetNotifyName_Implementation() const
{
	return "PauseAndRelease";
}

void UAN_MontagePause::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACharacter* ownerCharacter = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(ownerCharacter);

	
	
}
