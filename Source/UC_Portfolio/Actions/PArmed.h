

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PArmed.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams
(
	FArmedBeginOverlap,
	class AActor*, InAttacker,
	class AActor*, InAttackCauser,
	class AActor*, InOther,
	const bool, bFromSweep,
	const FHitResult&, SweepResult
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams
(
	FArmedEndOverlap,
	class AActor*, InAttacker,
	class AActor*, InAttackCauser,
	class AActor*, InOther
);


UCLASS()
class UC_PORTFOLIO_API APArmed : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APArmed();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public: //Equip, UnEquip
	UFUNCTION(BlueprintImplementableEvent)
		void OnEquip();

	UFUNCTION(BlueprintImplementableEvent)
		void OnUnequip();

public:
	UFUNCTION(BlueprintCallable)
		void OnCollision();
	void OnCollision(const FString& InCollisionName);
	UFUNCTION(BlueprintCallable)
		void OffCollision();

public:
	UFUNCTION(BlueprintImplementableEvent)
		void K2OnCollision();

	UFUNCTION(BlueprintImplementableEvent)
		void K2OnCollisionByName(const FString& InCollisionName);

	UFUNCTION(BlueprintImplementableEvent)
		void K2OffCollision();

public:
	UFUNCTION(BlueprintImplementableEvent)
		void K2ActiveNiagaraTrail();

	UFUNCTION(BlueprintImplementableEvent)
		void K2DeActiveNiagaraTrail();

public:
	UPROPERTY(BlueprintAssignable)
		FArmedBeginOverlap OnArmedBeginOverlap;
	UPROPERTY(BlueprintAssignable)
		FArmedEndOverlap OnArmedEndOverlap;

protected:// Socket, Attach
	UFUNCTION(BlueprintCallable)
		void AttachTo(FName InSocketName);

	UFUNCTION(BlueprintCallable)
		void AttachToCollision(class USceneComponent* InComponent, FName InSocketName);

protected:
	UPROPERTY(BlueprintReadOnly)
		class ACharacter* OwnerCharacter;

protected: //OverlapEvent
	UFUNCTION(BlueprintCallable)
		virtual void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	UPROPERTY(BlueprintReadWrite)
		class UShapeComponent* currentShapeComponent;

protected:
	UPROPERTY(EditDefaultsOnly)
		float HitOffset = 20.0f;

private: //Scene Root
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Scene;

	TArray<class UShapeComponent*> ShapeComponents;

	

};
