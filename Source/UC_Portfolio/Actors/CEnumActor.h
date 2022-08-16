#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CEnumActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFunctionDelegate);

//예시용 enum class
UENUM(BlueprintType)
enum class EGraphicQualityType : uint8
{
	Default_Graphic,
	Low_Graphic,
	Middle_Graphic,
	High_Graphic,
	Max_Graphic,
};

UCLASS()
class UC_PORTFOLIO_API ACEnumActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ACEnumActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	FFunctionDelegate OnDelegateEvent;

private:
	/// <summary>
	/// enum값을 문자로 바꿔주는 함수
	/// 설명은 함수 내부에 자세하게 되어있음.
	/// </summary>
	void PrintToEnum();

};
