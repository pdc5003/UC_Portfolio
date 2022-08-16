#include "Characters/PCommandBuffer.h"
#include "Global.h"
#include "GameFramework/Character.h"


UPCommandBuffer::UPCommandBuffer()
{
	
}

UPCommandBuffer::~UPCommandBuffer()
{
	if (queue.IsEmpty() == false)
		queue.Empty();
}

void UPCommandBuffer::BeginPlay(ACharacter* InCharacter)
{
	OwnerCharacter = InCharacter;

	CommandList.Add({ 1.0f });
	CommandList.Add({ -1.0f });
	CommandList.Add({ -1.0f,1.0f });
	CommandList.Add({ 1.0f,-1.0f });
}

void UPCommandBuffer::Tick(float DeltaTime)
{
	CheckTrue(bCommandCheck);
	CheckFalse(bClearTrigger);
	CheckTrue(queue.IsEmpty());

	ClearRate += DeltaTime;

	if (ClearRate > ClearDelay)
	{
		ClearRate = 0.0f;
		ClearQueue();
		return;
	}
}


void UPCommandBuffer::AddInputBuffer(const float& InBuffer)
{
	if (UKismetMathLibrary::NearlyEqual_FloatFloat(InBuffer, 0.0f, 0.01f))
	{
		bClearTrigger = true;
		return;
	}


	if (queue.IsEmpty() == true)
	{
		bClearTrigger = false;
		ClearRate = 0.0f;
		queue.Enqueue(InBuffer);
		lastInput = InBuffer;
		queueCount++;
		return;
	}

	if (UKismetMathLibrary::NearlyEqual_FloatFloat(InBuffer, lastInput, 0.01f))
		return;

	bClearTrigger = false;
	ClearRate = 0.0f;
	queue.Enqueue(InBuffer);
	lastInput = InBuffer;
	queueCount++;

	
}

void UPCommandBuffer::ClearQueue()
{
	queue.Empty();
	queueCount = 0;
}

void UPCommandBuffer::CheckCommandList(ECommandType& InCommandType)
{
	if (queue.IsEmpty())
	{
		InCommandType = ECommandType::Noraml;
		return;
	}

	bCommandCheck = true;
	bool bCorrect = true;
	int commandNumber = 0;
	for (int i = 0; i < CommandList.Num(); i++)
	{
		if (queueCount != CommandList[i].Num())
			continue;

		bCorrect = true;

		for (int j = 0; j < CommandList[i].Num(); j++)
		{
			float current = 0.0f;
			queue.Dequeue(current);
			queue.Enqueue(current);

			bCorrect &= (current == CommandList[i][j]);
		}

		if (bCorrect)
		{
			commandNumber = i;
			break;
		}
	}

	bCorrect ? InCommandType = (ECommandType)(commandNumber+1) : InCommandType = ECommandType::Noraml;
	ClearQueue();
	bCommandCheck = false;
}

