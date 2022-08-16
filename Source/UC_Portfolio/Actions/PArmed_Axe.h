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
	//애님 노티파에서 호출 할 함수들
	//AN_AxeThrowing에서 호출
	void AN_Detach();

	//AN_Axe_ThrowingEnd에서 호출
	void AN_ThrowingEnd();

	//AN_CatchEnd에서 호출
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
	//도끼 던졌을 시 업데이트 함수들, 델리게이트
	FThrowAxeUpdateDelegate OnThrowAxeUpdate;

	UFUNCTION()
		void UpdateRotateAxeMesh();
	UFUNCTION()
		void UpdateAxeTrace();

private:
	//도끼 회수 시 업데이트 함수, 델리게이트
	FRecallAxeUpdateDelegate OnRecallAxeUpdate;

	UFUNCTION()
		void UpdateReturnAxe();

	//베지어 곡선식을 이용한 도끼의 회수 동선 계산 함수
	FVector CalculateBezierCurve();

private:
	//애님 몽타주 재생
	void PlayAnimMontage(EMontageKey InKey);

private:
	//도끼가 타겟에 부딪혔을 때 도끼날 각도 보정
	void AxeIK();
	//도끼가 타겟과 부딪혔을 때 보정할 위치 값
	FVector AdjustAxeImpactLocation();
	//도끼가 타겟과 부딪혔을 때 보정할 회전 값
	FRotator AdjustAxeImpactRotation();

private:
	//도끼 트레일 켜기
	void PlayThrowTrail();
	//도끼 트레일 끄기
	void StopThrowTrail();
	//회수한 도끼를 잡았을 때 이펙트 재생
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
	bool bWasAim = false; //에임상태였는가?
	bool bCanRecall = false; //회수가능한가?
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
	//도끼의 마지막 위치 값
	FVector LastAxeLocation;

	//회수 시간 0~1사이로 나타내며 1인 경우 회수 완료
	float RecallTime = 0.0f;

	//도끼와 도끼를 던진 캐릭터간의 거리
	float DistanceFromOwner = 0.0f;

	//회수한 도끼를 잡았을 때 카메라 셰이크
	UPROPERTY(EditDefaultsOnly, Category = "AxeStatus")
		TSubclassOf<class UMatineeCameraShake> CamShake;

	//도끼를 던지는 힘
	UPROPERTY(EditDefaultsOnly, Category = "AxeStatus")
		float AxeThrowPower = 5000.0f;

	//도끼를 던졌을 때 도끼의 회전 속도
	UPROPERTY(EditDefaultsOnly, Category = "AxeStatus")
		float AxeRotationSpeed = 20.0f;

	//도끼를 회수 할 때 도끼의 Pitch값 회전 속도
	UPROPERTY(EditDefaultsOnly, Category = "AxeStatus")
		float AxeReturnPitchSpeed = 10.0f;

	//도끼를 회수 할 때 도끼의 Roll값 회전 속도
	UPROPERTY(EditDefaultsOnly, Category = "AxeStatus")
		float AxeReturnRollSpeed = 10.0f;

	//도끼의 가속도값
	UPROPERTY(EditDefaultsOnly, Category = "AxeStatus")
		float AxeAceelSpeed = 2.0f;

	//도끼를 회수할 때 거리에 따른 속도를 제한하기 위한 거리의 최대 값
	UPROPERTY(EditDefaultsOnly, Category = "AxeStatus")
		float MaxLimitDistance = 5000.0f;

private:
	UPROPERTY(EditDefaultsOnly, Category = "ThrowMontages")
		TMap<EMontageKey, FThwrowMontage> ThrowMontages;

private:
	AActor* Target;




};
