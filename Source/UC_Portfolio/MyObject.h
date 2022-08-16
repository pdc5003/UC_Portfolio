

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MyObject.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class UC_PORTFOLIO_API UMyObject : public UObject
{
	GENERATED_BODY()
	
public:
	UMyObject();
	~UMyObject();

public:
	UFUNCTION(BlueprintCallable, Category = "MyAudio", meta = (WorldContext = "WorldContextObject"))
		void PlayBgm(const UObject* WorldContextObject);


private:
	void PlayMusic(const UObject* WorldContextObject);

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USoundBase* bgmSoundBase;

	

};
