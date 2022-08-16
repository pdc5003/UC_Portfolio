#include "Actions/CSkillSideEffect_Around.h"
#include "Global.h"
#include "Actions/PArmed.h"
#include "Actions/PCombat.h"
#include "GameFramework/Character.h"
#include "Components/PCombatComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

ACSkillSideEffect_Around::ACSkillSideEffect_Around()
{
	CHelpers::CreateComponent(this, &Sphere, "Sphere", RootComponent);
	
}


void ACSkillSideEffect_Around::BeginPlay()
{
	Super::BeginPlay();
	PlayParticles();
}

void ACSkillSideEffect_Around::PlayParticles()
{
	Super::PlayParticles();

	CheckNull(NiagaraParticle);
	CheckNull(GetOwner());
	
	ACharacter* character = Cast<ACharacter>(GetOwner());
	FVector Location = character->GetMesh()->GetSocketLocation("Root");

	UNiagaraFunctionLibrary::SpawnSystemAtLocation
	(
		GetWorld(),
		NiagaraParticle,
		Location,
		GetActorRotation(),
		ParticleScale
	);
}

void ACSkillSideEffect_Around::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnComponentBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
	if (OtherActor == GetOwner()) return;
	

	UPCombatComponent* combat = CHelpers::GetComponent<UPCombatComponent>(GetOwner());
	CheckNull(combat);

	APArmed* armed = combat->GetCurrent()->GetArmed();
	CheckNull(armed);


	FHitResult result = SweepResult;
	result.Location = OtherActor->GetActorLocation();

	if (armed->OnArmedBeginOverlap.IsBound())
		armed->OnArmedBeginOverlap.Broadcast(GetOwner(), this, OtherActor, bFromSweep, result);

	
}

void ACSkillSideEffect_Around::OnDestruction(AActor* DestroyedActor)
{
	Super::OnDestruction(DestroyedActor);
	Sphere->DestroyComponent();
	Sphere = NULL;
}
