#include "Actions/CSkillSideEffect_ExpandCollision.h"
#include "Global.h"
#include "Actions/PArmed.h"
#include "Actions/PCombat.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Components/PCombatComponent.h"

ACSkillSideEffect_ExpandCollision::ACSkillSideEffect_ExpandCollision()
{
	CHelpers::CreateComponent(this, &Scene, "Scene", RootComponent);
	CHelpers::CreateComponent(this, &Box, "Box", Scene);
}

void ACSkillSideEffect_ExpandCollision::BeginPlay()
{
	Super::BeginPlay();
}

void ACSkillSideEffect_ExpandCollision::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnComponentBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if (OtherActor == GetOwner()) return;

	UPCombatComponent* combat = CHelpers::GetComponent<UPCombatComponent>(GetOwner());
	CheckNull(combat);

	APArmed* armed = combat->GetCurrent()->GetArmed();
	CheckNull(armed);


	FHitResult result = SweepResult;
	result.Location = Box->GetComponentLocation();

	if (armed->OnArmedBeginOverlap.IsBound())
		armed->OnArmedBeginOverlap.Broadcast(GetOwner(), this, OtherActor, bFromSweep, result);


}

void ACSkillSideEffect_ExpandCollision::OnDestruction(AActor* DestroyedActor)
{
	Super::OnDestruction(DestroyedActor);
}
