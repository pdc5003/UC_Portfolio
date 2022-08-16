#include "Characters/PAnimInstance.h"
#include "Global.h"
#include "GameFramework/Character.h"

void UPAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	ACharacter* character = Cast<ACharacter>(TryGetPawnOwner());
	CheckNull(character);
	
	UPCombatComponent* combatAction = CHelpers::GetComponent<UPCombatComponent>(character);
	CheckNull(combatAction);
	
	combatAction->OnCombatTypeChange.AddDynamic(this, &UPAnimInstance::OnActionTypeChanged);
}

void UPAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ACharacter* character = Cast<ACharacter>(TryGetPawnOwner());
	CheckNull(character);

	Speed = character->GetVelocity().Size2D();
	Direction = CalculateDirection(character->GetVelocity(), character->GetControlRotation());
	
	/* 떨어지고 있는 중이다라고 판정할 Z방향 가속도 값 */
	FMath::Abs(character->GetVelocity().Z) > 250.0f ? bFalling = true : bFalling = false;
	
}

void UPAnimInstance::OnAiming(bool bAim)
{
	bAiming = bAim;
}

void UPAnimInstance::OnThrow(bool bThrow)
{
	bThrowing = bThrow;
}

void UPAnimInstance::OnActionTypeChanged(ECombatType InPrevType, ECombatType InNewType)
{
	CombatType = InNewType;
}