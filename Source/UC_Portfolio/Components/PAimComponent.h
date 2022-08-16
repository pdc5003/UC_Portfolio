#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PAimComponent.generated.h"

DECLARE_DYNAMIC_DELEGATE(FAimingDelegate);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UC_PORTFOLIO_API UPAimComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPAimComponent();

public:
	void Initialize(class ACharacter* InCharacter);

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	const FORCEINLINE bool IsAiming() const { return bAim; }


private:
	FAimingDelegate OnAimingDelegate;


private:
	//���� Ȱ��ȭ ��Ȱ��ȭ , ����������Ʈ FAiming�� ����
	UFUNCTION()
		void Aiming(bool bAiming);
	void ZoomIn();
	void ZoomOut();

private:
	//���� �� ��, �� �ƿ� �Լ� FAimingDelegate�� ����
	UFUNCTION()
		void ZoomInUpdate();
	UFUNCTION()
		void ZoomOutUpdate();

private:
	class ACharacter* OwnerCharacter;
	class UCStateComponent* State;
	class USpringArmComponent* SpringArm;
	class UCameraComponent* Camera;
	class ACHUD* Hud;

private: //ZoomIn
	float ZoomInTargetArmLength = 200.0f;
	float ZooomInFieldOfView = 70.0f;
	FVector ZoomInSocketOffset = FVector(0.0f, 85.0f, 20.0f);
	bool ZoomInEnableCameraLag = false;

private: //ZoomOut
	float ZoomOutTargetArmLength;
	float ZooomOutFieldOfView;
	FVector ZoomOutSocketOffset;
	bool ZoomOutEnableCameraLag = true;

private:
	bool bAim;

	float Transition = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Duration")
		float Duration = 17.5f;
};
