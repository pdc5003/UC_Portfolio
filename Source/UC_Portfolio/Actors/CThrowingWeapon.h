#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CThrowingWeapon.generated.h"

DECLARE_DYNAMIC_DELEGATE(FShotWeapon);

UCLASS()
class UC_PORTFOLIO_API ACThrowingWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	ACThrowingWeapon();

public:
	void Initialize();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnStareTarget();
	UFUNCTION()
	void OnFireWeapon();

private:
	FShotWeapon ShootingDelegate;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Scene;

	UPROPERTY(EditDefaultsOnly)
		class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly)
		class UProjectileMovementComponent* Projectile;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UBoxComponent* Box;

	UPROPERTY(EditDefaultsOnly)
		class UParticleSystem* Particle;

private:
	UPROPERTY(EditDefaultsOnly)
		float LifeSpanTime = 5.0f;

	UPROPERTY(EditDefaultsOnly)
		float ThrowPower;

	float StareTime;

};
