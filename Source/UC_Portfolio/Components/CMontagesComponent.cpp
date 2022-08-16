#include "CMontagesComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"

UCMontagesComponent::UCMontagesComponent()
{

}

void UCMontagesComponent::BeginPlay()
{
	Super::BeginPlay();

	TArray<FMontageData*> datas;
	DataTable->GetAllRows<FMontageData>("", datas);
	
	for (int32 i = 0; i < (int32)EStateType::Max; i++)
	{
		for (FMontageData* data : datas)
		{
			if ((EStateType)i == data->Type)
			{
				Datas[i] = data;

				continue;
			}
		}//for(data)
	}//for(i)
}

void UCMontagesComponent::PlayRoll()
{
	PlayAnimMontage(EStateType::Roll);
}

void UCMontagesComponent::PlayBackstep()
{
	PlayAnimMontage(EStateType::Backstep);
}

void UCMontagesComponent::PlayHitted()
{
	PlayAnimMontage(EStateType::Hitted);
}

void UCMontagesComponent::PlayDead()
{
	PlayAnimMontage(EStateType::Dead);
}

void UCMontagesComponent::PlayAirBorne()
{
	PlayAnimMontage(EStateType::AirHitted);
}

void UCMontagesComponent::PlayAirFalling()
{
	PlayAnimMontage(EStateType::FallingEnd);
}

void UCMontagesComponent::PlayStun()
{
	PlayAnimMontage(EStateType::Stun);
}

void UCMontagesComponent::PlayDodge()
{
	PlayAnimMontage(EStateType::Dodge);
}

void UCMontagesComponent::PlayTeleport()
{
	PlayAnimMontage(EStateType::Teleport);
}

void UCMontagesComponent::PlayTeleport(FName InSectionName)
{
	PlayAnimMontage(EStateType::Teleport, InSectionName);
}

void UCMontagesComponent::PlayAnimMontage(EStateType InStateType)
{
	ACharacter* character = Cast<ACharacter>(GetOwner());
	
	const FMontageData* data = Datas[(int32)InStateType];
	if (!!data)
	{
		if (!!data->AnimMontage)
			character->PlayAnimMontage(data->AnimMontage, data->PlayRatio, data->StartSection);
	}
}

void UCMontagesComponent::PlayAnimMontage(EStateType InStateType, FName InSectionName)
{
	ACharacter* character = Cast<ACharacter>(GetOwner());

	const FMontageData* data = Datas[(int32)InStateType];
	if (!!data)
	{
		if (!!data->AnimMontage)
			character->PlayAnimMontage(data->AnimMontage, data->PlayRatio, InSectionName);
	}
}