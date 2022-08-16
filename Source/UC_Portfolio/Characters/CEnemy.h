#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/ICharacter.h"
#include "Components/CStateComponent.h"
#include "CEnemy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeadNotifier);

UCLASS()
class UC_PORTFOLIO_API ACEnemy : public ACharacter, public IICharacter
{
	GENERATED_BODY()

public:
	ACEnemy();

public:
	UPROPERTY(BlueprintAssignable)
	FDeadNotifier OnDeadNotifier;

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void PossessAI();

private:
	UPROPERTY(EditAnywhere, Category = "UserSettings")
		bool bDrawName = true;

	UPROPERTY(EditDefaultsOnly)
		float LaunchValue = 100.0f;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UWidgetComponent* EnemyWidget;

	UPROPERTY(VisibleDefaultsOnly)
		class UWidgetComponent* ExecutionWidget;

protected:
	UPROPERTY(VisibleDefaultsOnly)
		class UPCombatComponent* Combat;

	UPROPERTY(VisibleDefaultsOnly)
		class UCMontagesComponent* Montages;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStatusComponent* Status;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStateComponent* State;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Freeze")
		class UAnimMontage* FreezeMontages;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	FORCEINLINE bool IsDodgeAble() { return HitCount >= MaxHitCount; }

public:
	UFUNCTION(BlueprintCallable)
	void OnAction();
	void OnDashAction();
	void OnUpperAction();
public:
	UFUNCTION(BlueprintCallable)
		void OnTwoHand();

	UFUNCTION(BlueprintCallable)
		void OnKnife();

	UFUNCTION(BlueprintCallable)
		void OnKatana();

#pragma region StateTypeChange Delegate

private:
	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

private:
	void Hitted();
	void AirHitted();
	void Dead();
	void Stunning();
	void Dodge();
	void Executed();

#pragma endregion

#pragma region Inherited via IICharacter

public:
	virtual void Launch_Character(FVector InDirection, float InPower) override;
	virtual int SendDamage(struct FDoCombatData const& combatData, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Send_Freeze(const bool& InFreeze, const float& InDamage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Hit_End() override;
	virtual void End_Execution() override;
	virtual void Set_SuperArmor(bool InSuperArmor) override;
private:
	void End_Dead() override;

#pragma endregion

#pragma region AirBorne

private:
	bool BeginAirBorne(struct FDoCombatData const& IncombatData, FVector const& InDirection);
	void UpdateAirBorne();

private:
	bool bAirHit = false;

private:
	FVector AirDirection = FVector::ZeroVector;

private:
	float InAirGravity = 0.098f;
	float AerialStartTime = 0.0f;
	float FlightTime = 0.0f;

#pragma endregion

private:
	void BeginStun();

	UFUNCTION()
		void EndStun();

	UFUNCTION()
		void ExecutionDead();

protected:
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Material")
		class UMaterialInstanceConstant* Body;
	UPROPERTY(EditDefaultsOnly, Category = "Material")
		class UMaterialInstanceConstant* Logo;

	class UMaterialInstanceDynamic* BodyMaterial;
	class UMaterialInstanceDynamic* LogoMaterial;

private:
	class AController* DamageInstigator;

private:
	bool bSuperArmor = false;
	bool bFreeze = false;
	bool bDead = false;
private:
	UPROPERTY(EditDefaultsOnly)
		int32 MaxHitCount = 2;

	int32 HitCount = 0;

private:
	UPROPERTY(EditDefaultsOnly)
	float StunTime = 3.0f;
};
