#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStatusComponent.generated.h"

UENUM(BlueprintType)
enum class ECharacterSpeed : uint8
{
	Walk, Run, Sprint, Max,
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UC_PORTFOLIO_API UCStatusComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "UserSettings")
		float MaxHealth = 100;

	UPROPERTY(EditDefaultsOnly, Category = "UserSettings")
		float MaxStunLimit = 100;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Speed")
		float Speed[(int32)ECharacterSpeed::Max] = { 200, 400, 600 };

public:
	FORCEINLINE float GetHealth() { return Health; }
	FORCEINLINE float GetMaxHealth() { return MaxHealth; }

	FORCEINLINE float GetCurrentStun() { return CurStunGauage; }
	FORCEINLINE float GetMaxStunLimit() { return MaxStunLimit; }

	FORCEINLINE bool CanMove() { return bCanMove; }
	FORCEINLINE bool IsStun() { return CurStunGauage >= MaxStunLimit; }


	FORCEINLINE float GetWalkSpeed() { return Speed[(int32)ECharacterSpeed::Walk]; }
	FORCEINLINE float GetRunSpeed() { return Speed[(int32)ECharacterSpeed::Run]; }
	FORCEINLINE float GetSprintSpeed() { return Speed[(int32)ECharacterSpeed::Sprint]; }

public:
	UCStatusComponent();

	void SetMove();
	void SetStop();

	void SetSpeed(ECharacterSpeed InSpeed);

	void AddHealth(float InAmount);
	void SubHealth(float InAmount);

	void AddStun(float InAmount);
	void ResetStun();

protected:
	virtual void BeginPlay() override;

private:
	float Health;
	float CurStunGauage = 0;

	bool bCanMove = true;
};
