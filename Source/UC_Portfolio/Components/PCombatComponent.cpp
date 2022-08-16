#include "Components/PCombatComponent.h"
#include "Global.h"
#include "Actions/PCombat.h"
#include "Actions/PCombatData.h"
#include "Actions/PArmedEquipment.h"
#include "Actions/PArmed.h"
#include "Actions/PCombatAction.h"
#include "GameFramework/Character.h"
#include "Interface/IMelee.h"

UPCombatComponent::UPCombatComponent()
{
	
}


// Called when the game starts
void UPCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* character = Cast<ACharacter>(GetOwner());
	for (int32 i = 0; i < (int32)ECombatType::MAX; i++)
	{
		if (!!DataAssets[i])
		{
			DataAssets[i]->BeginPlay(character, &CombatDatas[i]);
			if (!!CombatDatas[i])
				CombatDatas[i]->GetCombatAction()->SetAimed(GetAimed());
			else
				CombatDatas[i] = nullptr;
		}
	}
}

void UPCombatComponent::GetBackCombatType()
{
	if (Type == Prev)
		return;

	Type = Prev;
}

void UPCombatComponent::DoAction()
{
	if (!!DataAssets[(int32)Type])
	{
		APCombatAction* combatAction = CombatDatas[(int32)Type]->GetCombatAction();

		if (combatAction != nullptr)
		{
			combatAction->DoAction();
			return;
		}
		
	}

}

void UPCombatComponent::DoDashAction()
{
	if (!!DataAssets[(int32)Type])
	{
		CheckNull(CombatDatas[(int32)Type]->GetCombatAction());
		IIMelee* melee = Cast<IIMelee>(CombatDatas[(int32)Type]->GetCombatAction());

		if (!!melee)
			melee->DoDashAction();
	}
}

void UPCombatComponent::DoUpperAction()
{
	if (!!DataAssets[(int32)Type])
	{

		if (IsValid(CombatDatas[(int32)Type]->GetCombatAction()) == false)
			return;

		CheckNull(CombatDatas[(int32)Type]->GetCombatAction());
		IIMelee* melee = Cast<IIMelee>(CombatDatas[(int32)Type]->GetCombatAction());

		if (!!melee)
			melee->DoUpperAction();
	}
}

void UPCombatComponent::DoSpecialAction()
{
	if (!!DataAssets[(int32)Type])
	{
		CheckNull(CombatDatas[(int32)Type]->GetCombatAction());

		IIMelee* melee = Cast<IIMelee>(CombatDatas[(int32)Type]->GetCombatAction());

		if (!!melee)
			melee->DoSpecialAction();
	}
}

void UPCombatComponent::DoSpecial2Action()
{
	if (!!DataAssets[(int32)Type])
	{
		CheckNull(CombatDatas[(int32)Type]->GetCombatAction());
		IIMelee* melee = Cast<IIMelee>(CombatDatas[(int32)Type]->GetCombatAction());

		if (!!melee)
			melee->DoSpecial2Action();
	}
}


void UPCombatComponent::DoAim(bool InAim)
{
	bAim = InAim;
	if (!!DataAssets[(int32)Type])
	{
		APCombatAction* combatAction = CombatDatas[(int32)Type]->GetCombatAction();

		if (!!combatAction)
		{
			InAim ? combatAction->OnAim() : combatAction->OffAim();

			if(OnAiming.IsBound())
				OnAiming.Broadcast(InAim);
		}
	}
}

void UPCombatComponent::Dead()
{
	for (UPCombat* data : CombatDatas)
	{
		if (!!data == false)
			continue;

		if (!!data->GetArmed() == false)
			continue;

		data->GetArmed()->OffCollision();
	}

}

void UPCombatComponent::End_Dead()
{
	for (int32 i = 0; i < (int32)ECombatType::MAX; i++)
	{
		if (!!DataAssets[i] && !!CombatDatas[i]->GetArmed())
			CombatDatas[i]->GetArmed()->Destroy();
	}
}

void UPCombatComponent::AbortAction()
{
	if (!!DataAssets[(int32)Type])
	{
		CheckNull(CombatDatas[(int32)Type]->GetCombatAction());
		APCombatAction* combatAction = CombatDatas[(int32)Type]->GetCombatAction();

		CheckNull(combatAction);
		combatAction->AbortDoAction();
			
	}
	DeActiveNiagaraTrail();
}

void UPCombatComponent::SetShield()
{
	if (!!CombatDatas[(int32)ECombatType::SHIELD] && CombatDatas[(int32)ECombatType::SHIELD]->GetEquipArmed())
	{
		CombatDatas[(int32)ECombatType::SHIELD]->GetEquipArmed()->Equip();
	}	
}


void UPCombatComponent::BeginBlock()
{
	Prev = Type;
	Type = ECombatType::SHIELD;
	DoAction();
}

void UPCombatComponent::EndBlock()
{
	if (!!DataAssets[(int32)ECombatType::SHIELD])
	{
		if (!!CombatDatas[(int32)ECombatType::SHIELD]->GetCombatAction())
		{
			IIMelee* melee = Cast<IIMelee>(CombatDatas[(int32)ECombatType::SHIELD]->GetCombatAction());

			if (!!melee)
				melee->DoActionCancle();
		}
	}
}

void UPCombatComponent::CounterAction()
{
	if (!!DataAssets[(int32)Type])
	{
		if (!!CombatDatas[(int32)Type]->GetCombatAction())
		{
			IIMelee* melee = Cast<IIMelee>(CombatDatas[(int32)Type]->GetCombatAction());

			if (!!melee)
				melee->DoCounterAction();
		}
	}
}


void UPCombatComponent::ActiveNiagaraTrail()
{
	if (!!DataAssets[(int32)Type])
	{
		CheckNull(CombatDatas[(int32)Type]->GetCombatAction());
		APArmed* armed = CombatDatas[(int32)Type]->GetArmed();
		CheckNull(armed);
		armed->K2ActiveNiagaraTrail();
	}
}

void UPCombatComponent::DeActiveNiagaraTrail()
{
	if (!!DataAssets[(int32)Type])
	{
		CheckNull(CombatDatas[(int32)Type]->GetCombatAction());
		APArmed* armed = CombatDatas[(int32)Type]->GetArmed();
		CheckNull(armed);
		armed->K2DeActiveNiagaraTrail();
	}
}

void UPCombatComponent::SetHideWeapon(bool bHide)
{
	if (!!DataAssets[(int32)Type])
	{
		CheckNull(CombatDatas[(int32)Type]->GetCombatAction());
		APArmed* armed = CombatDatas[(int32)Type]->GetArmed();
		CheckNull(armed);
		armed->SetHidden(bHide);
	}
}

APArmed* UPCombatComponent::GetAxe() const
{
	return CombatDatas[(int32)ECombatType::AXE]->GetArmed();
}



void UPCombatComponent::SetAxeMode()
{
	SetMode(ECombatType::AXE);
}

void UPCombatComponent::SetBareHandMode()
{
	SetMode(ECombatType::BAREHAND);
}

void UPCombatComponent::SetKnifeMode()
{
	SetMode(ECombatType::KNIFE);
}

void UPCombatComponent::SetTwoHandMode()
{
	SetMode(ECombatType::TWOHAND);
}

void UPCombatComponent::SetKatanaMode()
{
	SetMode(ECombatType::KATANA);
}

void UPCombatComponent::SetMode(ECombatType InType)
{
	/*if (Type == InType)
		return;*/

	if (!!CombatDatas[(int32)InType])
	{
		if(!!CombatDatas[(int32)Type])
			CombatDatas[(int32)Type]->GetEquipArmed()->UnEquip();
	}

	if (!!CombatDatas[(int32)InType] && CombatDatas[(int32)InType]->GetEquipArmed())
	{
		CombatDatas[(int32)InType]->GetEquipArmed()->Equip();
		ChangeType(InType);
	}
}

void UPCombatComponent::ChangeType(ECombatType InNewType)
{
	//ECombatType prev = Type;
	Prev = Type;
	Type = InNewType;
	
	if (OnCombatTypeChange.IsBound())
		OnCombatTypeChange.Broadcast(Prev, InNewType);
	
}

