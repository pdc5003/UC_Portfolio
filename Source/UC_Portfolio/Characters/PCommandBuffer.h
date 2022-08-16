#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PCommandBuffer.generated.h"

UENUM(BlueprintType)
enum class ECommandType : uint8
{
	Noraml,Dash,Upper,Special,Special2,Max
};

UCLASS()
class UC_PORTFOLIO_API UPCommandBuffer : public UObject
{
	GENERATED_BODY()

public:
	UPCommandBuffer();
	~UPCommandBuffer();
public:
	void BeginPlay(class ACharacter* InCharacter);
	void Tick(float DeltaTime);
	void AddInputBuffer(const float& InBuffer);
	void CheckCommandList(ECommandType& InCommandType);

private:
	void ClearQueue();

private:
	class ACharacter* OwnerCharacter;
	FTimerHandle ClearQueueTimer;

private:
	TQueue<float> queue;
	TArray<TArray<float>> CommandList;

	float lastInput = 0.0f;
	float queueCount;
	float ClearDelay = 0.07f;
	float ClearRate = 0.0f;

	bool bEmpty = true;
	bool bClearTrigger = false;
	bool bCommandCheck = false;
};
