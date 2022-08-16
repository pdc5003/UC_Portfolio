

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PCombat.generated.h"

UENUM(BlueprintType)
enum class EAttackStyle : uint8
{
	Normal, Upper, Blast, Max
};

USTRUCT(BlueprintType)
struct FEquipArmedData
{
	GENERATED_BODY()
		
public:
	UPROPERTY(EditAnywhere)
		class UAnimMontage* AnimMontage;

	UPROPERTY(EditAnywhere)
		float PlayRatio = 1.0f;

	UPROPERTY(EditAnywhere)
		FName StartSection;

	UPROPERTY(EditAnywhere)
		bool bCanMove = false;

	UPROPERTY(EditAnywhere)
		bool bPawnControl = true;
};

USTRUCT(BlueprintType)
struct FDoCombatData : public FEquipArmedData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		float Power = 5.0f;

	UPROPERTY(EditAnywhere)
		float StunPower = 5.0f;

	UPROPERTY(EditAnywhere)
		float HitStop;

	UPROPERTY(EditAnywhere)
		float Angle = 10.0f;

	UPROPERTY(EditAnywhere)
		float PushPower = 300.0f;

	UPROPERTY(EditAnywhere)
		EAttackStyle AttackStyle = EAttackStyle::Normal;

	UPROPERTY(EditAnywhere)
		class UParticleSystem* Effect;

	UPROPERTY(EditAnywhere)
		FTransform ParticleTransform;

	UPROPERTY(EditAnywhere)
		class UNiagaraSystem* NiagaraEffect;

	UPROPERTY(EditAnywhere)
		FTransform NiagaraTransform;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UMatineeCameraShake> ShakeClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACSkillSideEffect> SideEffectClass;
	
};



UCLASS()
class UC_PORTFOLIO_API UPCombat : public UObject
{
	GENERATED_BODY()
	
public:
	friend class UPCombatData;

public:
	FORCEINLINE class APArmed* GetArmed() { return Armed; }
	FORCEINLINE class APArmedEquipment* GetEquipArmed() { return AremdEquipment; }
	FORCEINLINE class APCombatAction* GetCombatAction() { return CombatAction; }


private:
	class APArmed* Armed;
	class APArmedEquipment* AremdEquipment;
	class APCombatAction* CombatAction;
};
