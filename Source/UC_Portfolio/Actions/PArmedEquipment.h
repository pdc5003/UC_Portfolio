

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PArmedEquipment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FArmedEquipmentDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FArmedUnEquipmentDelegate);

UCLASS()
class UC_PORTFOLIO_API APArmedEquipment : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APArmedEquipment();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:// Equip, Unequip
	UFUNCTION(BlueprintNativeEvent)
		void Equip();
	void Equip_Implementation();

	UFUNCTION(BlueprintNativeEvent)
		void UnEquip();
	void UnEquip_Implementation();

public:
	UPROPERTY(BlueprintAssignable)
		FArmedEquipmentDelegate OnArmedEquipmentDelegate;

	UPROPERTY(BlueprintAssignable)
		FArmedUnEquipmentDelegate OnArmedUnequipmentDelegate;

protected:
	UPROPERTY(BlueprintReadOnly)
		class ACharacter* OwnerCharacter;
};
