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
	//ó���� �Ϸ�Ǿ��°�?
	bool ExecutionFinisher();
	//������ ������ ���¿��� ó���� �ϴ� ��� ������ ���� ȸ���� ���� ��������Ʈ
	FQuickRecallAxe OnQuickRecall;

public:///AnimNotify Call Function
	
	//���� ó�� ��� ���ϴ� ��Ÿ��
	void Next_FinisherMontage(); 
	//���� ó�� ��� ���ϴ� ��Ÿ��
	void Next_FinishedMontage(); 
	//ó�� ��Ÿ�� �Ϸ�� ȣ��
	void End_Finisher(); 

private:
	//ó�� ������ Ÿ�ٵ� ���
	void GetTargets();
	//���� ����� ó�� ��� ���
	void GetMostCloseTarget(class TArray<AActor*>& InActors);

private:
	//ó�� ���� ĳ���� ��Ÿ�� �÷���
	void DoPlayFinisher();
	//ó�� ���ϴ� ĳ���� ��Ÿ�� �÷���
	void DoPlayFinished();
	//ó�� ���� ������ ���� �ֺ� ĳ���͵� CustomTimeDilation�� �̿��� ����
	void FreezeAround(bool bFreeze);

private:
	//ó�� �� ������ ����
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
