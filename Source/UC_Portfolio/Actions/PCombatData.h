#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PCombat.h"
#include "PCombatData.generated.h"


UCLASS()
class UC_PORTFOLIO_API UPCombatData : public UDataAsset
{
	GENERATED_BODY()

public:
	void BeginPlay(class ACharacter* InOwner, class UPCombat** OutCombatAction);
	//void Destroy();

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<class APArmed> ArmedClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<class APArmedEquipment> ArmedEquipmentClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FEquipArmedData ArmedEquipData;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<class APCombatAction> CombatActionClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TArray<FDoCombatData> DoDefaultCombatDatas;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TArray<FDoCombatData> DoSkillCombatDatas;


private:
	FString GetLabelName(class ACharacter* InOwnerCharacter, FString InName);
};
