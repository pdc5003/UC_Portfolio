#include "Actors/CThrowingWeapon.h"
#include "Global.h"
#include "Actions/PCombat.h"
#include "Characters/ICharacter.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystem.h"

ACThrowingWeapon::ACThrowingWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	CHelpers::CreateComponent<USceneComponent>(this, &Scene, "Default", RootComponent);
	CHelpers::CreateComponent<UStaticMeshComponent>(this, &Mesh, "Mesh", Scene);
	CHelpers::CreateComponent<UBoxComponent>(this, &Box, "Box", Scene);

	CHelpers::CreateActorComponent<UProjectileMovementComponent>(this, &Projectile, "Projectile");
	Projectile->ProjectileGravityScale = 0.0f;
	Projectile->bInitialVelocityInLocalSpace = false;
	Projectile->Velocity = FVector::ZeroVector;
	Projectile->bAutoActivate = false;
	Projectile->Deactivate();
}

void ACThrowingWeapon::Initialize()
{
	Box->OnComponentBeginOverlap.AddDynamic(this, &ACThrowingWeapon::OnComponentBeginOverlap);
	ShootingDelegate.BindDynamic(this, &ACThrowingWeapon::OnStareTarget);
}

void ACThrowingWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ACThrowingWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ShootingDelegate.IsBound())
		ShootingDelegate.Execute();
}

void ACThrowingWeapon::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IGenericTeamAgentInterface* otherAgent = Cast<IGenericTeamAgentInterface>(OtherActor);
	IGenericTeamAgentInterface* ownerAgent = Cast<IGenericTeamAgentInterface>(GetOwner());

	CheckNull(otherAgent);
	CheckNull(ownerAgent);

	if (otherAgent->GetGenericTeamId() == ownerAgent->GetGenericTeamId())
		return;

	IICharacter* other = Cast<IICharacter>(OtherActor);
	int sendDamageSuccess = -1;

	if (!!other)
	{
		FDoCombatData combatData;
		combatData.Angle = 0;
		combatData.AttackStyle = EAttackStyle::Normal;
		combatData.PushPower = 100;
		combatData.Power = 3.0f;

		FDamageEvent e;

		sendDamageSuccess = other->SendDamage(combatData, e, GetOwner()->GetInstigatorController(), this);
	}
	
	if (sendDamageSuccess == 0 || sendDamageSuccess == 1)
	{
		if (!!Particle)
		{
			FTransform particleTransform;
			particleTransform.AddToTranslation(GetActorLocation());
			particleTransform.SetScale3D(FVector(0.35f, 0.35f, 0.35f));
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, particleTransform);
		}

		Mesh->SetVisibility(false);
		Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	
}

void ACThrowingWeapon::OnStareTarget()
{
	StareTime += GetWorld()->GetDeltaSeconds();
	
	FVector targetLocation =  UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();

	FRotator rotate = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), targetLocation);
	SetActorRotation(rotate);

	if (StareTime >= 0.5f)
	{
		ShootingDelegate.Clear();
		ShootingDelegate.BindDynamic(this, &ACThrowingWeapon::OnFireWeapon);
	}
}

void ACThrowingWeapon::OnFireWeapon()
{
	ShootingDelegate.Clear();
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	FVector direction = FQuat(GetActorRotation()).GetForwardVector();
	Projectile->Velocity = direction * ThrowPower;
	Projectile->Activate();
}

