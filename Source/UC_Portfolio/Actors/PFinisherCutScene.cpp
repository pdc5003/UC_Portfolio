#include "Actors/PFinisherCutScene.h"
#include "Global.h"
#include "LevelSequence.h"
#include "LevelSequenceActor.h"
#include "CineCameraActor.h"

// Sets default values
APFinisherCutScene::APFinisherCutScene()
{
}


void APFinisherCutScene::BeginPlay()
{
	//Spawn CineCamera
	{
		FTransform transform = GetTransform();

		FActorSpawnParameters params;
		params.Owner = this;

		CutSceneCamera = GetWorld()->SpawnActor<ACineCameraActor>(ACineCameraActor::StaticClass(), transform, params);
		CutSceneCamera->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	}

	Super::BeginPlay();
}

void APFinisherCutScene::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	StopAndRestore();
}

/// <summary>
/// ������ ���͸� �����ϰ� BeginPlay���� ������ �ó׸�ƽ ī�޶� ���͸�
/// ó�� �� �������� �ִ� "FinisherCamera" �±׸� ã�� �ǽð����� ���ε��� �� �� ���
/// </summary>
/// <param name="InSequence">����� ó���� ������</param>
void APFinisherCutScene::PlayFinisherScene(ULevelSequence* InSequence)
{
	//Create SequencerActor
	{
		CheckNull(InSequence);
		FMovieSceneSequencePlaybackSettings Settings;
		Settings.bAutoPlay = false;
		Settings.StartTime = 0.0f;
		ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), InSequence, Settings, SequenceActor);
		CheckNull(SequenceActor);
	}

	//Binding New Camera
	{
		FMovieSceneObjectBindingID bindingID = SequenceActor->FindNamedBinding("FinisherCamera");
		SequenceActor->AddBinding(bindingID, CutSceneCamera);
	}

	CheckNull(CutSceneCamera);
	CheckNull(SequenceActor);
	SequenceActor->SequencePlayer->Play();
	bPlaying = true;
}

void APFinisherCutScene::StopAndRestore()
{
	CheckFalse(bPlaying);

	float curTime = static_cast<float>(SequenceActor->SequencePlayer->GetCurrentTime().Time.FrameNumber.Value);
	float endTime = static_cast<float>(SequenceActor->SequencePlayer->GetEndTime().Time.FrameNumber.Value);

	float remainRatio = 1.0f - curTime / endTime;

	if (remainRatio < 0.1f)
	{
		bPlaying = false;
		SequenceActor->SequencePlayer->Stop();
		SequenceActor->ResetBindings();
		SequenceActor->Destroy();
		SequenceActor = nullptr;
		CutSceneCamera->SetActorRelativeLocation(FVector::ZeroVector);
		CutSceneCamera->SetActorRelativeRotation(FRotator::ZeroRotator);
	}
}

