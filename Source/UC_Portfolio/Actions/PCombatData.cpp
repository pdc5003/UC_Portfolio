#include "Actions/PCombatData.h"
#include "Global.h"
#include "PArmed.h"
#include "PArmedEquipment.h"
#include "PCombatAction.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"

void UPCombatData::BeginPlay(class ACharacter* InOwner, class UPCombat** OutCombatAction)
{
	FTransform transform;

	APArmed* Armed = nullptr;
	if (!!ArmedClass)
	{
		Armed = InOwner->GetWorld()->SpawnActorDeferred<APArmed>(ArmedClass, transform, InOwner);
		//Armed->SetActorLabel(GetLabelName(InOwner, "Aremd"));
		UGameplayStatics::FinishSpawningActor(Armed, transform);
	}

	APArmedEquipment* ArmedEquipment = nullptr;
	if (!!ArmedEquipmentClass)
	{
		ArmedEquipment = InOwner->GetWorld()->SpawnActorDeferred<APArmedEquipment>(ArmedEquipmentClass, transform, InOwner);
		ArmedEquipment->AttachToComponent(InOwner->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
		//ArmedEquipment->SetActorLabel(GetLabelName(InOwner, "Equipment"));
		UGameplayStatics::FinishSpawningActor(ArmedEquipment, transform);

		if (!!Armed)
		{
			ArmedEquipment->OnArmedEquipmentDelegate.AddDynamic(Armed, &APArmed::OnEquip);
			ArmedEquipment->OnArmedUnequipmentDelegate.AddDynamic(Armed, &APArmed::OnUnequip);
		}
	}

	APCombatAction* CombatAction = nullptr;
	if (!!CombatActionClass)
	{
		CombatAction = InOwner->GetWorld()->SpawnActorDeferred<APCombatAction>(CombatActionClass, transform, InOwner);
		CombatAction->AttachToComponent(InOwner->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
		CombatAction->SetDefaultCombatData(DoDefaultCombatDatas);
		CombatAction->SetSkillCombatData(DoSkillCombatDatas);
		UGameplayStatics::FinishSpawningActor(CombatAction, transform);

		if (!!Armed)
		{
			Armed->OnArmedBeginOverlap.AddDynamic(CombatAction, &APCombatAction::OnBeginOverlap);
			Armed->OnArmedEndOverlap.AddDynamic(CombatAction, &APCombatAction::OnEndOverlap);
		}

	}

	*OutCombatAction = NewObject<UPCombat>();
	(*OutCombatAction)->Armed = Armed;
	(*OutCombatAction)->AremdEquipment = ArmedEquipment;
	(*OutCombatAction)->CombatAction = CombatAction;
}


FString UPCombatData::GetLabelName(ACharacter* InOwnerCharacter, FString InName)
{
	FString name;
	//name.Append(InOwnerCharacter->GetActorLabel());
	name.Append("_");
	name.Append(InName);
	name.Append("_");
	name.Append(GetName().Replace(L"DA_", L""));

	return name;
}
