#include "CDelegateActor.h"

ACDelegateActor::ACDelegateActor()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ACDelegateActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACDelegateActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACDelegateActor::FirstFunction(const FVoidFunction& DelParam)
{
	VoidDelegate = DelParam;

	if (VoidDelegate.IsBound())
		VoidDelegate.Execute();
		
}

void ACDelegateActor::SecondFunction(const int32& intVal, const FOneParamFunction& DelParam)
{
	OneParamDelegate = DelParam;

	if (OneParamDelegate.IsBound())
		OneParamDelegate.Execute(intVal);
}

