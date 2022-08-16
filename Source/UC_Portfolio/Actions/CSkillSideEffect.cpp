#include "Actions/CSkillSideEffect.h"
#include "Global.h"
#include "Components/ShapeComponent.h"

// Sets default values
ACSkillSideEffect::ACSkillSideEffect()
{
	InitialLifeSpan = LifeSpanTime;
}

void ACSkillSideEffect::BeginPlay()
{
	OnDestroyed.AddDynamic(this, &ACSkillSideEffect::OnDestruction);

	Super::BeginPlay();
	
	TArray<UShapeComponent*>components;
	GetComponents<UShapeComponent>(components);
	for (UShapeComponent* component : components)
	{
		if (!!component)
			component->OnComponentBeginOverlap.AddDynamic(this, &ACSkillSideEffect::OnComponentBeginOverlap);
	}

	SetLifeSpan(LifeSpanTime);
}

