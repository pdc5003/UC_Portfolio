#include "Components/PFinisherComponent.h"
#include "Global.h"
#include "Actions/PFinisherData.h"
#include "Animation/AnimMontage.h"
#include "Actors/PFinisherCutScene.h"
#include "Components/CStateComponent.h"
#include "GameFramework/Character.h"
#include "LevelSequence.h"

UPFinisherComponent::UPFinisherComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UPFinisherComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	CutSceneActor = OwnerCharacter->GetWorld()->SpawnActor<APFinisherCutScene>(CutSceneActorClass);
}


void UPFinisherComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

bool UPFinisherComponent::ExecutionFinisher()
{
	if (FinishDataAsset == nullptr)
		return false;
	
	if (OnQuickRecall.IsBound())
		OnQuickRecall.Execute();

	GetTargets();

	if (Target == nullptr)
		return false;

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(Target);

	if (state == nullptr)
		return false;

	if (state->IsStunMode() == false)
		return false;

	state->SetExecutionMode();
	
	float dist = (OwnerCharacter->GetActorLocation() - Target->GetActorLocation()).Size();

	APlayerController* controller = Cast<APlayerController>(OwnerCharacter->GetController());

	if (!!controller)
		OwnerCharacter->DisableInput(controller);

	//Rotate Each Other
	{
		FRotator rotate = UKismetMathLibrary::FindLookAtRotation(Target->GetActorLocation(), OwnerCharacter->GetActorLocation());
		Target->SetActorRotation(FRotator(0.0f, rotate.Yaw, 0.0f));
	
		FRotator rotator = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), Target->GetActorLocation());
		FRotator current = OwnerCharacter->GetControlRotation();
		OwnerCharacter->GetController()->SetControlRotation(rotator);
		OwnerCharacter->SetActorRotation(FQuat(rotator));
	}
	
	if(!bTestIndexHold)
		Index = UKismetMathLibrary::RandomIntegerInRange(0, 2000) % FinishDataAsset->FinisherDatas.Num();

	//Set Position OwnerCharacter
	{
		FVector targetFwd = FQuat(Target->GetActorRotation()).GetForwardVector();
		FVector ownerLocation = targetFwd * FinishDataAsset->FinisherDatas[Index].Interval + Target->GetActorLocation();

		OwnerCharacter->SetActorLocation(ownerLocation);
	}

	//LevelSequencer
	{
		if (!!CutSceneActor)
		{
			FVector location = OwnerCharacter->GetActorLocation();
			FRotator rotator = OwnerCharacter->GetActorRotation();
			CutSceneActor->SetActorLocation(location);
			CutSceneActor->SetActorRotation(FQuat(rotator));

			if (!!FinishDataAsset->FinisherDatas[Index].Sequence)
			{
				ULevelSequence* temp = Cast<ULevelSequence>(FinishDataAsset->FinisherDatas[Index].Sequence);
				CutSceneActor->PlayFinisherScene(temp);
			}
		}
	}

	//Play FinisherMontage
	{
		DoPlayFinisher();
		DoPlayFinished();
	}

	FreezeAround(true);
	
	

	return true;
}

void UPFinisherComponent::End_Finisher()
{
	FreezeAround(false);
	if (bTestIndexHold)
		Index++;
	Target = nullptr;

	FinisherMontageIndex = 0;
	FinishedMontageIndex = 0;

	APlayerController* controller = Cast<APlayerController>(OwnerCharacter->GetController());
	CheckNull(controller);
	OwnerCharacter->EnableInput(controller);

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	CheckNull(state);
	state->SetIdleMode();
}

void UPFinisherComponent::GetTargets()
{

	TArray<TEnumAsByte<EObjectTypeQuery>> objects;
	objects.Add(EObjectTypeQuery::ObjectTypeQuery3);

	TArray<AActor*> ignores;
	ignores.Add(OwnerCharacter);

	TArray<AActor*> outActors;

	UKismetSystemLibrary::SphereOverlapActors
	(
		OwnerCharacter->GetWorld(),
		OwnerCharacter->GetActorLocation(),
		FinisherRadius,
		objects,
		nullptr,
		ignores,
		outActors
	);

	if (outActors.Num() <= 0)
		return;

	GetMostCloseTarget(outActors);
}

void UPFinisherComponent::GetMostCloseTarget(TArray<AActor*>& InActors)
{
	float Distance = 1000000.0f;

	for (AActor* actor : InActors)
	{
		float curDist = (actor->GetActorLocation() - OwnerCharacter->GetActorLocation()).Size();

		if (curDist < Distance)
		{
			Distance = curDist;
			Target = Cast<ACharacter>(actor);
		}
	}
}

void UPFinisherComponent::DoPlayFinisher()
{
	UAnimMontage* ownerMontage = FinishDataAsset->FinisherDatas[Index].FinisherMontageDatas[FinisherMontageIndex].AnimMontage;
	
	float ratio = FinishDataAsset->FinisherDatas[Index].FinisherMontageDatas[FinisherMontageIndex].PlayRatio;
	FName section = FinishDataAsset->FinisherDatas[Index].FinisherMontageDatas[FinisherMontageIndex].StartSection;
	
	OwnerCharacter->StopAnimMontage();
	OwnerCharacter->PlayAnimMontage(ownerMontage, ratio,section);

	FinisherMontageIndex++;
}

void UPFinisherComponent::DoPlayFinished()
{
	UAnimMontage* targetMontage = FinishDataAsset->FinisherDatas[Index].FinishedMontageDatas[FinishedMontageIndex].AnimMontage;
	float ratio = FinishDataAsset->FinisherDatas[Index].FinishedMontageDatas[FinishedMontageIndex].PlayRatio;
	FName section = FinishDataAsset->FinisherDatas[Index].FinishedMontageDatas[FinishedMontageIndex].StartSection;
	
	Target->StopAnimMontage();
	Target->PlayAnimMontage(targetMontage,ratio,section);

	FinishedMontageIndex++;
}

void UPFinisherComponent::FreezeAround(bool bFreeze)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> objects;
	objects.Add(EObjectTypeQuery::ObjectTypeQuery3);

	TArray<AActor*> ignores;
	ignores.Add(OwnerCharacter);
	ignores.Add(Target);

	TArray<AActor*> outActors;

	UKismetSystemLibrary::SphereOverlapActors
	(
		OwnerCharacter->GetWorld(),
		OwnerCharacter->GetActorLocation(),
		2000.0f,
		objects,
		nullptr,
		ignores,
		outActors
	);

	for (AActor* actor : outActors)
	{
		if (bFreeze == true)
			actor->CustomTimeDilation = 0.01f;
		else
			actor->CustomTimeDilation = 1.0f;
	}
}


void UPFinisherComponent::Next_FinisherMontage()
{
	DoPlayFinisher();
}

void UPFinisherComponent::Next_FinishedMontage()
{
	DoPlayFinished();
}