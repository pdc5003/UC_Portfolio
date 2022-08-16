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
	//런타임 중 시네마틱 카메라 액터 바인딩 및 처형씬 재생
	void PlayFinisherScene(class ULevelSequence* InSequence);

private:
	//처형씬 종료 및 파괴
	void StopAndRestore();

private:
	//처형씬을 재생할 시퀀스 액터
	class ALevelSequenceActor* SequenceActor;
	//처형씬에 바인딩할 시네마틱 카메라 액터
	class ACineCameraActor* CutSceneCamera;
	//처형씬이 플레이 중인지 알아보기 위한 변수
	bool bPlaying = false;
};
