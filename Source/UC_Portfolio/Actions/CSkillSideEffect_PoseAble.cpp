#include "Actions/CSkillSideEffect_PoseAble.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/PoseableMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"

ACSkillSideEffect_PoseAble::ACSkillSideEffect_PoseAble()
{
	CHelpers::CreateComponent<UPoseableMeshComponent>(this, &PoseableMesh, "PoseableMesh", RootComponent);
}


void ACSkillSideEffect_PoseAble::BeginPlay()
{
	Super::BeginPlay();
	UMaterialInstanceConstant* Ghost = NULL;

	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&Ghost, "MaterialInstanceConstant'/Game/Materials/MI_Ghost.MI_Ghost'");
	GhostMaterial = UMaterialInstanceDynamic::Create(Ghost, this);

	OwnerCharacter = Cast<ACharacter>(GetOwner());

	PoseableMesh->SetWorldTransform(OwnerCharacter->GetMesh()->GetComponentTransform());

	PoseableMesh->SetSkeletalMesh(OwnerCharacter->GetMesh()->SkeletalMesh);
	PoseableMesh->SetMaterial(0, GhostMaterial);
	PoseableMesh->SetMaterial(1, GhostMaterial);
	PoseableMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PoseableMesh->CopyPoseFromSkeletalComponent(OwnerCharacter->GetMesh());
}

