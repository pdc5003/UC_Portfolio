#include "Characters/CAIController_Boss.h"
#include "Global.h"
#include "Components/CBehaviorComponent.h"

void ACAIController_Boss::OnPossess(APawn* InPawn)
{
	PatternSetting();
	Super::OnPossess(InPawn);
}

void ACAIController_Boss::PatternSetting()
{
	TArray<FBossPattern*> data;
	PatternTable->GetAllRows<FBossPattern>("", data);
	
	//Pattern Setting
	{
		UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(this);
		int32 listIndx = 0;
		for (const FBossPattern* item : data)
		{
			TArray<FPatternBehavior> FuncArray;
			for (int32 i = 0; i < item->Patterns.Num(); i++)
			{
				FString left;
				FString right;
				item->Patterns[i].Split("_", &left, &right);
				int32 patternID = FCString::Atoi(*left);

				FPatternBehavior behaviorFunc;

				switch (patternID)
				{
					case 0:
						{
							behaviorFunc.BindDynamic(behavior, &UCBehaviorComponent::SetIdleMode);
							FuncArray.Add(behaviorFunc);
						}
						break;

					case 2:
						{
							behaviorFunc.BindDynamic(behavior, &UCBehaviorComponent::SetRoamingMode);
							FuncArray.Add(behaviorFunc);
						}
						break;

					case 3:
						{
							behaviorFunc.BindDynamic(behavior, &UCBehaviorComponent::SetTeleportMode);
							FuncArray.Add(behaviorFunc);
						}
						break;

					case 4:
						{
							behaviorFunc.BindDynamic(behavior, &UCBehaviorComponent::SetActionMode);
							FuncArray.Add(behaviorFunc);
						}
						break;

					case 5:
						{
							behaviorFunc.BindDynamic(behavior, &UCBehaviorComponent::SetDashActionMode);
							FuncArray.Add(behaviorFunc);
						}
						break;

					case 6:
						{
							behaviorFunc.BindDynamic(behavior, &UCBehaviorComponent::SetUpperActionMode);
							FuncArray.Add(behaviorFunc);
						}
						break;
				}
			}//for (int32 i = 0; i < item->Patterns.Num(); i++)

			PatternList.Add(listIndx, FuncArray);
			listIndx++;
		}//for (const FBossPattern* item : data)

	}//Pattern Setting

	CurrPatternID = UKismetMathLibrary::RandomIntegerInRange(0, 1000) % PatternList.Num();
	CurrBehaviorModeIdx = 0;

	
}

void ACAIController_Boss::ExecutePattern()
{
	if (CurrBehaviorModeIdx >= PatternList[CurrPatternID].Num())
	{
		CurrPatternID = UKismetMathLibrary::RandomIntegerInRange(0, 1000) % PatternList.Num();
		CurrBehaviorModeIdx = 0;
	}

	if (PatternList[CurrPatternID][CurrBehaviorModeIdx].IsBound())
	{
		PatternList[CurrPatternID][CurrBehaviorModeIdx].Execute();
		CurrBehaviorModeIdx++;
	}

}