#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PFinisherCutScene.generated.h"

UCLASS()
class UC_PORTFOLIO_API APFinisherCutScene : public AActor
{
	GENERATED_BODY()
	
public:	
	APFinisherCutScene();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	//��Ÿ�� �� �ó׸�ƽ ī�޶� ���� ���ε� �� ó���� ���
	void PlayFinisherScene(class ULevelSequence* InSequence);

private:
	//ó���� ���� �� �ı�
	void StopAndRestore();

private:
	//ó������ ����� ������ ����
	class ALevelSequenceActor* SequenceActor;
	//ó������ ���ε��� �ó׸�ƽ ī�޶� ����
	class ACineCameraActor* CutSceneCamera;
	//ó������ �÷��� ������ �˾ƺ��� ���� ����
	bool bPlaying = false;
};
