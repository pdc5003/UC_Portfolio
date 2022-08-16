#include "Notifies/AN_EndFinisher.h"
#include "Global.h"
#include "Components/PFinisherComponent.h"

FString UAN_EndFinisher::GetNotifyName_Implementation() const
{
	return "EndFinisher";
}

void UAN_EndFinisher::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UPFinisherComponent* finisher = CHelpers::GetComponent<UPFinisherComponent>(MeshComp->GetOwner());
	CheckNull(finisher);

	finisher->End_Finisher();
}