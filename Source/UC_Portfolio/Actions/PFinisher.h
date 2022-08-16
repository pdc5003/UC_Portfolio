#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PFinisher.generated.h"
USTRUCT(BlueprintType)
struct FFinishMontageData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		class UAnimMontage* AnimMontage;

	UPROPERTY(EditAnywhere)
		float PlayRatio = 1.0f;

	UPROPERTY(EditAnywhere)
		FName StartSection;
};

USTRUCT(BlueprintType)
struct FFinisherData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		TArray<FFinishMontageData> FinisherMontageDatas;

	UPROPERTY(EditAnywhere)
		TArray<FFinishMontageData> FinishedMontageDatas;

	UPROPERTY(EditAnywhere)
		class UMovieSceneSequence* Sequence;

	UPROPERTY(EditAnywhere)
		float Interval = 200.0f;
};


UCLASS()
class UC_PORTFOLIO_API UPFinisher : public UObject
{
	GENERATED_BODY()
	
};
