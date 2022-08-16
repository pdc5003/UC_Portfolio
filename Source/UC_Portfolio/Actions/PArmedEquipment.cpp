#include "Actions/PArmedEquipment.h"
#include "Global.h"
#include "GameFramework/Character.h"

// Sets default values
APArmedEquipment::APArmedEquipment()
{
 
}

// Called when the game starts or when spawned
void APArmedEquipment::BeginPlay()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());

	Super::BeginPlay();
	
}

void APArmedEquipment::Equip_Implementation()
{
	if (OnArmedEquipmentDelegate.IsBound())
		OnArmedEquipmentDelegate.Broadcast();
}

void APArmedEquipment::UnEquip_Implementation()
{
	if (OnArmedUnequipmentDelegate.IsBound())
		OnArmedUnequipmentDelegate.Broadcast();
}




