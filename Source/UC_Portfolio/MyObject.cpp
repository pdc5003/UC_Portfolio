#include "Global.h"
#include "MyObject.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"

UMyObject::UMyObject() 
{
	CHelpers::GetAsset<USoundBase>(&bgmSoundBase, "SoundWave'/Game/Sounds/morban_motel.morban_motel'");
}
UMyObject::~UMyObject() 
{
}

void UMyObject::PlayBgm(const UObject* WorldContextObject)
{
	if (!!bgmSoundBase)
	{
		PlayMusic(WorldContextObject);
	}
}

void UMyObject::PlayMusic(const UObject* WorldContextObject)
{

	//FString str;

	//UWorld* CurrentWorld = GEngine->GetWorld();

	//if (CurrentWorld == nullptr)
	//{
	//	CLog::Print("World is Null", 1, 5.0f,FColor::Emerald);
	//	return;
	//}

	//CurrentWorld->GetName(str);
	//CLog::Print(str, 1, 5.0f);
	//UGameplayStatics::SpawnSound2D(this, bgmSoundBase);
	
}