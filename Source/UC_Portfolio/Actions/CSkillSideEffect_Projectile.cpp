#include "Actions/CSkillSideEffect_Projectile.h"
#include "Global.h"
#include "Actions/PArmed.h"
#include "Actions/PCombat.h"
#include "Components/SceneComponent.h"
#include "Components/PCombatComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ACSkillSideEffect_Projectile::ACSkillSideEffect_Projectile()
{
	CHelpers::CreateComponent<USceneComponent>(this, &Scene, "Default", RootComponent);
	CHelpers::CreateActorComponent<UProjectileMovementComponent>(this, &Projectile, "Projectile");
	Projectile->ProjectileGravityScale = 0.0f;
	Projectile->bInitialVelocityInLocalSpace = false;
	Projectile->Velocity = FVector::ZeroVector;
	Projectile->bAutoActivate = false;
	Projectile->Deactivate();
}


void ACSkillSideEffect_Projectile::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(LifeSpanTime);
	FVector direction = FQuat(GetActorRotation()).GetForwardVector();
	Projectile->Velocity = direction * ThrowPower;
	Projectile->Activate();
}

void ACSkillSideEffect_Projectile::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnComponentBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (OtherActor == GetOwner()) return;

	UPCombatComponent* combat = CHelpers::GetComponent<UPCombatComponent>(GetOwner());
	CheckNull(combat);

	APArmed* armed = combat->GetCurrent()->GetArmed();
	CheckNull(armed);


	FHitResult result = SweepResult;
	result.Location = GetActorLocation();

	if (armed->OnArmedBeginOverlap.IsBound())
		armed->OnArmedBeginOverlap.Broadcast(GetOwner(), this, OtherActor, bFromSweep, result);


	//IGenericTeamAgentInterface* otherAgent = Cast<IGenericTeamAgentInterface>(OtherActor);
	//IGenericTeamAgentInterface* ownerAgent = Cast<IGenericTeamAgentInterface>(GetOwner());

	//CheckNull(otherAgent);
	//CheckNull(ownerAgent);

	//if (otherAgent->GetGenericTeamId() == ownerAgent->GetGenericTeamId())
	//	return;


	//ACharacter* Other = Cast<ACharacter>(OtherActor);
	//IICharacter* character = Cast<IICharacter>(Other);

	//CheckNull(character);

	//FDoCombatData combatData;
	//combatData.Angle = 0;
	//combatData.AttackStyle = EAttackStyle::Normal;
	//combatData.PushPower = 100;
	//combatData.Power = 3.0f;

	//FDamageEvent e;
	//
	//character->SendDamage(combatData, e, GetOwner()->GetInstigatorController(), this);

}

void ACSkillSideEffect_Projectile::OnDestruction(AActor* DestroyedActor)
{
	Super::OnDestruction(DestroyedActor);
}
