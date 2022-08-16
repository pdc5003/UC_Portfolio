#include "Actions/PCombatAction_Knife.h"
#include "Global.h"
#include "Actions/CSkillSideEffect.h"
#include "GameFramework/Character.h"

void APCombatAction_Knife::DoSideEffect()
{
	CheckFalse(CurCombatDatas.IsValidIndex(Index));
	CheckNull(CurCombatDatas[Index].SideEffectClass);

	FTransform transform;
	transform.SetLocation(OwnerCharacter->GetMesh()->GetSocketLocation("Throw_Dagger"));
	
	FRotator rotate = OwnerCharacter->GetMesh()->GetSocketRotation("Throw_Dagger");
	rotate.Roll = 0.0f;
	rotate.Pitch = 0.0f;
	transform.SetRotation(FQuat(rotate));

	transform.SetScale3D(FVector::OneVector);
	FActorSpawnParameters params;
	params.Owner = OwnerCharacter;

	ACSkillSideEffect* sideEffect = GetWorld()->SpawnActorDeferred<ACSkillSideEffect>
		(
			CurCombatDatas[Index].SideEffectClass.Get(),
			transform,
			OwnerCharacter,
			NULL,
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
			);
	UGameplayStatics::FinishSpawningActor(sideEffect, transform);
}