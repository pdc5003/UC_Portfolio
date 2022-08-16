#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/ICharacter.h"
#include "Components/CStateComponent.h"
#include "GenericTeamAgentInterface.h"
#include "PPlayer.generated.h"


UCLASS()
class UC_PORTFOLIO_API APPlayer : public ACharacter, public IICharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	APPlayer();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



private: //Team ID
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		uint8 TeamID = 0;

private: //Camera
	UPROPERTY(VisibleDefaultsOnly)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly)
		class UCameraComponent* Camera;

private: //Component
	UPROPERTY(VisibleDefaultsOnly)
		class UPCombatComponent* Combat;

	

	UPROPERTY(VisibleDefaultsOnly)
		class UCStateComponent* State;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStatusComponent* Status;

	UPROPERTY(VisibleDefaultsOnly)
		class UCOptionComponent* Option;

	UPROPERTY(VisibleDefaultsOnly)
		class UPAbilityComponent* Ability;

	UPROPERTY(VisibleDefaultsOnly)
		class UCMontagesComponent* Montages;

	UPROPERTY(VisibleDefaultsOnly)
		class UPAimComponent* AimComp;

	UPROPERTY(VisibleDefaultsOnly)
		class UPFinisherComponent* Finisher;

	UPROPERTY(VisibleDefaultsOnly)
		class UCLockOnComponent* LockOn;

private:
	UPROPERTY(EditDefaultsOnly)
		class UAnimMontage* DodgeMontage[4];

private: //Widget;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UCUserWidget_Health> PlayerWidgetClass;

	class UCUserWidget_Health* PlayerWidget;

public:
	UFUNCTION(BlueprintCallable)
		void ShowPlayerHUD();

	UFUNCTION(BlueprintCallable)
		void HidePlayerHUD();

private:
	void OnAction();
	void OnAim();
	void OffAim();
	void OnFinisher();
	void OnLockOn();
	void OffLockOn();
	void OnBlocking();
	void OffBlocking();

private:
	void OnDodge();

private:
	void OnMoveForward(float InAxis);
	void OnMoveRight(float InAxis);
	void OnHorizontalLook(float InAxis);
	void OnVerticalLook(float InAxis);
public:
	void OnAxe();
	void OnBareHand();

#pragma region Inherited via IICharacter
public: //ICharacter
	virtual void Launch_Character(FVector InDirection, float InPower) override;
	virtual int SendDamage(struct FDoCombatData const& combatData, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Hit_End() override;
	virtual void Set_SuperArmor(bool InSuperArmor) override;
#pragma endregion

#pragma region Inhretied via IGenericTeamAgentInterface
public:
	virtual FGenericTeamId GetGenericTeamId() const override;

#pragma endregion

#pragma region StateTypeChange Delegate

private:
	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

private:
	void Hitted();
	void AirHitted();
	void Dead();
	void Stunning();

#pragma endregion

#pragma region Blocking Properties
	
private:
	void OnBlockingSideEffect();

private:
	UFUNCTION()
		void ReleaseBlockingHitStop();

private:
	UPROPERTY(EditDefaultsOnly)
		float BlockingHitStop = 0.035f;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UMatineeCameraShake> BlockingShake;
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
	class AController* DamageInstigator;

private:
	UPROPERTY()
	class UPCommandBuffer* CommandBuffer;

private:
	UPROPERTY(EditDefaultsOnly)
		float GuardBackPower = 300.0f;
	
private: //Material
	UPROPERTY(EditDefaultsOnly, Category = "Material")
		class UMaterialInstanceConstant* Body;
	UPROPERTY(EditDefaultsOnly, Category = "Material")
		class UMaterialInstanceConstant* Logo;

	class UMaterialInstanceDynamic* BodyMaterial;
	class UMaterialInstanceDynamic* LogoMaterial;

	private:
		UPROPERTY(EditDefaultsOnly)
			float HealthRecoverPower = 2.0f;

private: 
	float StartTime;
	float Delay = 0.1f;

	bool bRootMotionLaunch = false;
	bool bBlock = false;
	bool bSuperArmor = false;

private:
	UPROPERTY(VisibleDefaultsOnly)
		TSubclassOf<class UMyObject> MyObject;

private:
	class UMyObject* MyTestObj;
};
