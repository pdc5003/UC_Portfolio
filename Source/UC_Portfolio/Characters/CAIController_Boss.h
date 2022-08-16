#pragma once

#include "CoreMinimal.h"
#include "Characters/CAIController.h"
#include "Engine/DataTable.h"
#include "CAIController_Boss.generated.h"

DECLARE_DYNAMIC_DELEGATE(FPatternBehavior);

USTRUCT(BlueprintType)
struct FBossPattern : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		TArray<FString> Patterns;

};

UCLASS()
class UC_PORTFOLIO_API ACAIController_Boss : public ACAIController
{
	GENERATED_BODY()
	
public:
	void ExecutePattern();

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	void PatternSetting();

private:
	UPROPERTY(EditDefaultsOnly)
		UDataTable* PatternTable;

private:
	TMap<int32, TArray<FPatternBehavior>> PatternList;
	
	int32 CurrPatternID;
	int32 CurrBehaviorModeIdx;
	
};
