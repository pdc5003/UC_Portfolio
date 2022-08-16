

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actions/PCombat.h"
#include "PCombatAction.generated.h"


UCLASS()
class UC_PORTFOLIO_API APCombatAction : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APCombatAction();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// Set Datas
	FORCEINLINE void SetDefaultCombatData(const TArray<FDoCombatData>& InData) { DefaultCombatDatas = InData; }
	FORCEINLINE void SetSkillCombatData(const TArray<FDoCombatData>& InData) { SkillCombatDatas = InData; }
	FORCEINLINE void SetAimed(const bool* InAim) { bAimed = InAim; }
public:
	// Combo, Aim, AttackEnd
	virtual void DoAction() {}
	virtual void Next_DoAction() {}
	virtual void End_DoAction() {}
	virtual void AbortDoAction() {}

	virtual void OnAim() {}
	virtual void OffAim() {}

public:
	virtual void HookCurrentCombatData(const float& Power, const float& HitStop, const float& Angle, const float& PushPower, int32 AttackStyle) {}

public:
	/*
	* ���� ���ݿ� ���� BeginOverlap �̺�Ʈ
	* ���� ���ݿ� ���� EndOverlap �̺�Ʈ
	* ���� ������ ���� ��� ����Ŭ����(PArmed)���� ������ ó�� �� ��
	*/
	UFUNCTION()
		virtual void OnBeginOverlap(class AActor* InAttacker, class AActor* InCauser, class AActor* InOther, const bool bFromSweep, const FHitResult& SweepResult) {}
	
	UFUNCTION()
		virtual void OnEndOverlap(class AActor* InAttacker, class AActor* InAttackCauser, class AActor* InOther) {}

protected:
	TArray<FDoCombatData> DefaultCombatDatas;
	TArray<FDoCombatData> SkillCombatDatas;

	//���� �Ĺ����
	TArray<FDoCombatData> CurCombatDatas;

protected:
	UPROPERTY(BlueprintReadOnly)
		class ACharacter* OwnerCharacter;

	UPROPERTY(BlueprintReadOnly)
		class UCStateComponent* State;

	UPROPERTY(BlueprintReadOnly)
		class UCStatusComponent* Status;

protected: //������ �ߴ°��� ���� �������� PCombationComponent�� bAim ���� �ּҰ�����
	const bool* bAimed;
	
};
