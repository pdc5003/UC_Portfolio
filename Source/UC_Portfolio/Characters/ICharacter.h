#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ICharacter.generated.h"


UINTERFACE(MinimalAPI)
class UICharacter : public UInterface
{
	GENERATED_BODY()
};

class UC_PORTFOLIO_API IICharacter
{
	GENERATED_BODY()

public:
	/// <summary>
	/// -1 : 데미지 전달 실패
	/// 0 : 데미지 전달 성공
	/// 1 : 데미지 전달 무효(가드)
	/// </summary>
	virtual int SendDamage(struct FDoCombatData const& combatData , struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) = 0;
	virtual void Send_Freeze(const bool& InFreeze, const float& InDamage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) {};
	virtual void Hit_End() = 0;
	virtual void End_Dead() {};
	virtual void End_Execution() {};
	virtual void Launch_Character(FVector InDirection, float InPower) {};
	virtual void Set_SuperArmor(bool InSuperArmor) {};
};
