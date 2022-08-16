

#pragma once

#include "CoreMinimal.h"
#include "Characters/CEnemy_AI.h"
#include "CEnemy_AI_Boss.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHideStuckOtherArmed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FShowStuckOtherArmed);

UCLASS()
class UC_PORTFOLIO_API ACEnemy_AI_Boss : public ACEnemy_AI
{
	GENERATED_BODY()

public:
	ACEnemy_AI_Boss();

protected:
	virtual void BeginPlay() override;

public:
	void OnTeleport();
	void OffTeleport();

public://AnimNotify
	void Begin_Teleport();
	void End_Teleport();

public:
	virtual void Send_Freeze(const bool& InFreeze, const float& InDamage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:
	void SpawnSummonSword();
	void GetRandomLocation(FVector& InLocation);

public:
	void UnRegistStuckArmed();

private:
	UFUNCTION()
		void OnBossStateTypeChanged(EStateType InPrevType, EStateType InNewType);

public:
	FHideStuckOtherArmed OnHideStuckArmed;
	FShowStuckOtherArmed OnShowStuckArmed;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	class UAnimMontage* TeleportMontage;

private:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class ACThrowingWeapon> SummonSword;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* SpPoint01;

	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* SpPoint02;

	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* SpPoint03;
};
