#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CEnumActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFunctionDelegate);

//���ÿ� enum class
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
	/// enum���� ���ڷ� �ٲ��ִ� �Լ�
	/// ������ �Լ� ���ο� �ڼ��ϰ� �Ǿ�����.
	/// </summary>
	void PrintToEnum();

};
