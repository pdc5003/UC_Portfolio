#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/CStateComponent.h"
#include "Engine/DataTable.h"
#include "CMontagesComponent.generated.h"

USTRUCT(BlueprintType)
struct FMontageData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		EStateType Type;

	UPROPERTY(EditAnywhere)
		class UAnimMontage* AnimMontage;

	UPROPERTY(EditAnywhere)
		float PlayRatio = 1.0f;

	UPROPERTY(EditAnywhere)
		FName StartSection;

	UPROPERTY(EditAnywhere)
		bool bCanMove;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UC_PORTFOLIO_API UCMontagesComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "DataTable")
		UDataTable* DataTable;

public:	
	UCMontagesComponent();

	void PlayRoll();
	void PlayBackstep();
	void PlayHitted();
	void PlayDead();
	void PlayAirBorne();
	void PlayAirFalling();
	void PlayStun();
	void PlayDodge();
	void PlayTeleport();
	void PlayTeleport(FName InSectionName);

protected:
	virtual void BeginPlay() override;	

private:
	void PlayAnimMontage(EStateType InStateType);
	void PlayAnimMontage(EStateType InStateType, FName InSectionName);

private:
	FMontageData* Datas[(int32)EStateType::Max];
};
