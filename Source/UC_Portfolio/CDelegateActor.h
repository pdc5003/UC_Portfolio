#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CDelegateActor.generated.h"

DECLARE_DYNAMIC_DELEGATE(FVoidFunction);
DECLARE_DYNAMIC_DELEGATE_OneParam
(
	FOneParamFunction, const int32&, intVal
);

UCLASS()
class UC_PORTFOLIO_API ACDelegateActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ACDelegateActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable)
		void FirstFunction(const FVoidFunction& DelParam);
	UFUNCTION(BlueprintCallable)
		void SecondFunction(const int32& intVal, const FOneParamFunction& DelParam);

	FVoidFunction VoidDelegate;
	FOneParamFunction OneParamDelegate;

};
