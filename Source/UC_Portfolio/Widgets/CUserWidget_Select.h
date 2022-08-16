#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_Select.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUserWidget_Select_Clicked);

USTRUCT(BlueprintType)
struct FUserWidget_Select_Data
{
	GENERATED_BODY()

public:
	class UCUserWidget_SelectItem* Item;
	FUserWidget_Select_Clicked OnUserWidget_Select_Clicked;
};

UCLASS()
class UC_PORTFOLIO_API UCUserWidget_Select : public UUserWidget
{
	GENERATED_BODY()

public:
	FORCEINLINE FUserWidget_Select_Data& GetData(uint32 InIndex) { return Datas[InIndex]; }

protected:
	virtual void NativeConstruct() override;

public:
	void Clicked(FString InName);
	void Hovered(FString InName);
	void Unhovered(FString InName);

private:
	TArray<FUserWidget_Select_Data> Datas;
	
};
