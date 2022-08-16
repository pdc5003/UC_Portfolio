#include "Notifies/AN_NextFinisher.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/PFinisherComponent.h"

FString UAN_NextFinisher::GetNotifyName_Implementation() const
{
	return "NextFinisher";
}

void UAN_NextFinisher::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UPFinisherComponent* finisher = CHelpers::GetComponent<UPFinisherComponent>(MeshComp->GetOwner());
	CheckNull(finisher);

	finisher->Next_FinisherMontage();
}

