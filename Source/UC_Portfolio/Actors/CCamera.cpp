#include "Actors/CCamera.h"
#include "Global.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
ACCamera::ACCamera()
{
	PrimaryActorTick.bCanEverTick = true;

	//CreateComponent(AActor* InActor, T** InComponent, FName InName, USceneComponent* InParent = NULL)
	CHelpers::CreateComponent<USceneComponent>(this, &Scene, "Scene", RootComponent);
	CHelpers::CreateComponent<UCameraComponent>(this, &Camera, "Camera", Scene);

}

void ACCamera::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

