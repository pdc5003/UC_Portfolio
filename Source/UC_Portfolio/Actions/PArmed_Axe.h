#pragma once

#include "CoreMinimal.h"
#include "Actions/PArmed.h"
#include "PArmed_Axe.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FThrowAxeUpdateDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRecallAxeUpdateDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSetAxe);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSetBareHand);


USTRUCT()
struct FThwrowMontage
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		class UAnimMontage* AnimMontage;

	UPROPERTY(EditAnywhere)
		float PlayRatio = 1.0f;

	UPROPERTY(EditAnywhere)
		FName StartSection;
};

UENUM(BlueprintType)
enum class EMontageKey : uint8
{
	Aim, Throw, Recall, Cacth, Max,
};

UENUM(BlueprintType)
enum class EAxeState : uint8
{
	Idle, Throwing, Stick, Recalling, Catch, Max
};

UCLASS()
class UC_PORTFOLIO_API APArmed_Axe : public APArmed
{
	GENERATED_BODY()

public:
	APArmed_Axe();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	void AxeAction();
	void AxeAim(const bool& bAim);

public:
	//�ִ� ��Ƽ�Ŀ��� ȣ�� �� �Լ���
	//AN_AxeThrowing���� ȣ��
	void AN_Detach();

	//AN_Axe_ThrowingEnd���� ȣ��
	void AN_ThrowingEnd();

	//AN_CatchEnd���� ȣ��
	void AN_CatchEnd();

protected:
	virtual void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)override;

private:
	void AxeThrowing();
	void AxeRecalling();
	void Detach();

private:
	UFUNCTION()
		void QuickRecall();
private:
	//Trace Variables
	FVector HitLocation;
	FVector ImpactNormal;
	FVector AxeEndPoint;

	FName BoneName;

	const float TraceDistance = 10000.0f;

private:
	FSetAxe OnSetAxe;
	FSetBareHand OnSetBareHand;


private:
	//���� ������ �� ������Ʈ �Լ���, ��������Ʈ
	FThrowAxeUpdateDelegate OnThrowAxeUpdate;

	UFUNCTION()
		void UpdateRotateAxeMesh();
	UFUNCTION()
		void UpdateAxeTrace();

private:
	//���� ȸ�� �� ������Ʈ �Լ�, ��������Ʈ
	FRecallAxeUpdateDelegate OnRecallAxeUpdate;

	UFUNCTION()
		void UpdateReturnAxe();

	//������ ����� �̿��� ������ ȸ�� ���� ��� �Լ�
	FVector CalculateBezierCurve();

private:
	//�ִ� ��Ÿ�� ���
	void PlayAnimMontage(EMontageKey InKey);

private:
	//������ Ÿ�ٿ� �ε����� �� ������ ���� ����
	void AxeIK();
	//������ Ÿ�ٰ� �ε����� �� ������ ��ġ ��
	FVector AdjustAxeImpactLocation();
	//������ Ÿ�ٰ� �ε����� �� ������ ȸ�� ��
	FRotator AdjustAxeImpactRotation();

private:
	//���� Ʈ���� �ѱ�
	void PlayThrowTrail();
	//���� Ʈ���� ����
	void StopThrowTrail();
	//ȸ���� ������ ����� �� ����Ʈ ���
	void PlayCatchParticle();

private:
	UFUNCTION()
		void HideAxeMesh();

	UFUNCTION()
		void ShowAxeMesh();

private:
	//Axe Mesh
	class UMeshComponent* Mesh;

	//Throw, Catch ParticleComponent 
	//Control At Runtime;
	class UParticleSystemComponent* ThrowParticleComponent;
	class UParticleSystemComponent* CatchParticleComponent;

	UPROPERTY(EditDefaultsOnly)
		class UParticleSystem* ThrowParticle;

	UPROPERTY(EditDefaultsOnly)
		class UParticleSystem* CatchParticle;

	//SubSceneComponent;
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* SubRoot;

	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* LodgePoint;

	UPROPERTY(VisibleDefaultsOnly)
		class UProjectileMovementComponent* ProjectileMovement;


private:
	bool bWasAim = false; //���ӻ��¿��°�?
	bool bCanRecall = false; //ȸ�������Ѱ�?
	UFUNCTION()
	void EnableRecall();

private:
	UFUNCTION(BlueprintCallable)
	const FORCEINLINE bool IsAxeIdle() const { return AxeState == EAxeState::Idle; }
	const FORCEINLINE bool IsAxeThrowing() const { return AxeState == EAxeState::Throwing; }
	const FORCEINLINE bool IsAxeStick() const { return AxeState == EAxeState::Stick; }
	const FORCEINLINE bool IsAxeRecalling() const { return AxeState == EAxeState::Recalling; }

	void SetAxeIdle();
	void SetAxeThrowing();
	void SetAxeStick();
	void SetAxeRecalling();
	void SetAxeState(EAxeState InNewState);

	EAxeState AxeState = EAxeState::Idle;
	

private:
	//������ ������ ��ġ ��
	FVector LastAxeLocation;

	//ȸ�� �ð� 0~1���̷� ��Ÿ���� 1�� ��� ȸ�� �Ϸ�
	float RecallTime = 0.0f;

	//������ ������ ���� ĳ���Ͱ��� �Ÿ�
	float DistanceFromOwner = 0.0f;

	//ȸ���� ������ ����� �� ī�޶� ����ũ
	UPROPERTY(EditDefaultsOnly, Category = "AxeStatus")
		TSubclassOf<class UMatineeCameraShake> CamShake;

	//������ ������ ��
	UPROPERTY(EditDefaultsOnly, Category = "AxeStatus")
		float AxeThrowPower = 5000.0f;

	//������ ������ �� ������ ȸ�� �ӵ�
	UPROPERTY(EditDefaultsOnly, Category = "AxeStatus")
		float AxeRotationSpeed = 20.0f;

	//������ ȸ�� �� �� ������ Pitch�� ȸ�� �ӵ�
	UPROPERTY(EditDefaultsOnly, Category = "AxeStatus")
		float AxeReturnPitchSpeed = 10.0f;

	//������ ȸ�� �� �� ������ Roll�� ȸ�� �ӵ�
	UPROPERTY(EditDefaultsOnly, Category = "AxeStatus")
		float AxeReturnRollSpeed = 10.0f;

	//������ ���ӵ���
	UPROPERTY(EditDefaultsOnly, Category = "AxeStatus")
		float AxeAceelSpeed = 2.0f;

	//������ ȸ���� �� �Ÿ��� ���� �ӵ��� �����ϱ� ���� �Ÿ��� �ִ� ��
	UPROPERTY(EditDefaultsOnly, Category = "AxeStatus")
		float MaxLimitDistance = 5000.0f;

private:
	UPROPERTY(EditDefaultsOnly, Category = "ThrowMontages")
		TMap<EMontageKey, FThwrowMontage> ThrowMontages;

private:
	AActor* Target;




};
