#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PFinisherComponent.generated.h"

DECLARE_DYNAMIC_DELEGATE(FQuickRecallAxe);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UC_PORTFOLIO_API UPFinisherComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPFinisherComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	//처형이 완료되었는가?
	bool ExecutionFinisher();
	//도끼가 던져진 상태에서 처형을 하는 경우 도끼의 빠른 회수를 위한 델리게이트
	FQuickRecallAxe OnQuickRecall;

public:///AnimNotify Call Function
	
	//다음 처형 모션 행하는 몽타주
	void Next_FinisherMontage(); 
	//다음 처형 모션 당하는 몽타주
	void Next_FinishedMontage(); 
	//처형 몽타주 완료시 호출
	void End_Finisher(); 

private:
	//처형 가능한 타겟들 얻기
	void GetTargets();
	//가장 가까운 처형 대상 얻기
	void GetMostCloseTarget(class TArray<AActor*>& InActors);

private:
	//처형 가해 캐릭터 몽타주 플레이
	void DoPlayFinisher();
	//처형 당하는 캐릭터 몽타주 플레이
	void DoPlayFinished();
	//처형 씬이 나오는 동안 주변 캐릭터들 CustomTimeDilation을 이용해 정지
	void FreezeAround(bool bFreeze);

private:
	//처형 씬 데이터 에셋
	UPROPERTY(EditDefaultsOnly, Category = "DataAssets")
		class UPFinisherData* FinishDataAsset;

private:
	UPROPERTY(EditDefaultsOnly)
		float FinisherRadius = 200.0f;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class APFinisherCutScene> CutSceneActorClass;
private:
	class ACharacter* Target;
	class ACharacter* OwnerCharacter;
	class APFinisherCutScene* CutSceneActor;

	FVector2D Direction;

	UPROPERTY(EditDefaultsOnly)
		bool bTestIndexHold = false;

	UPROPERTY(EditDefaultsOnly)
		int32 Index = 0;

	int32 FinisherMontageIndex = 0;
	int32 FinishedMontageIndex = 0;
};
