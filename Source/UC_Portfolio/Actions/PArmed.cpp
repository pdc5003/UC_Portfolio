#include "Actions/PArmed.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/SceneComponent.h"
#include "Components/ShapeComponent.h"
#include "NiagaraComponent.h"

// Sets default values
APArmed::APArmed()
{
	CHelpers::CreateComponent<USceneComponent>(this, &Scene, "Scene");
}

// Called when the game starts or when spawned
void APArmed::BeginPlay()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());

	GetComponents<UShapeComponent>(ShapeComponents);
	for (UShapeComponent* component : ShapeComponents)
	{
		component->OnComponentBeginOverlap.AddDynamic(this, &APArmed::OnComponentBeginOverlap);
		component->OnComponentEndOverlap.AddDynamic(this, &APArmed::OnComponentEndOverlap);
	}

	OffCollision();

	Super::BeginPlay();

}

void APArmed::OnCollision()
{
	if(ShapeComponents.Num()<0)
		GetComponents<UShapeComponent>(ShapeComponents);

	for (UShapeComponent* component : ShapeComponents)
	{
		if (!!component)
		{
			currentShapeComponent = component;
			component->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}
	}
}

void APArmed::OnCollision(const FString& InCollisionName)
{
	for (UShapeComponent* component : ShapeComponents)
	{
		if (!!component)
		{
			if (component->GetName() == InCollisionName)
			{
				currentShapeComponent = component;
				component->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				break;
			}
		}
	}
}

void APArmed::OffCollision()
{
	for (UShapeComponent* component : ShapeComponents)
	{
		if (!!component)
			component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}


void APArmed::AttachTo(FName InSocketName)
{
	AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), InSocketName);
}

void APArmed::AttachToCollision(USceneComponent* InComponent, FName InSocketName)
{
	InComponent->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), InSocketName);
}

void APArmed::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(OwnerCharacter == OtherActor);
	
	FHitResult result = SweepResult;
	result.bBlockingHit = true;
	if (!!currentShapeComponent)
	{
		FVector hitPoint = (OtherActor->GetActorLocation() - currentShapeComponent->GetComponentLocation()).GetSafeNormal();
		result.Location = currentShapeComponent->GetComponentLocation() + (hitPoint* HitOffset);
	}

	if (OnArmedBeginOverlap.IsBound())
		OnArmedBeginOverlap.Broadcast(OwnerCharacter, this, OtherActor, bFromSweep, result);

}

void APArmed::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OnArmedEndOverlap.IsBound())
		OnArmedEndOverlap.Broadcast(OwnerCharacter, this, OtherActor);
}



