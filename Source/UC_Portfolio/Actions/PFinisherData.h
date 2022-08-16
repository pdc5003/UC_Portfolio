

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Actions/PFinisher.h"
#include "PFinisherData.generated.h"

/**
 * 
 */
UCLASS()
class UC_PORTFOLIO_API UPFinisherData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TArray<FFinisherData> FinisherDatas;
};
