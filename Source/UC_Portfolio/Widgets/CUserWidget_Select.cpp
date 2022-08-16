#include "CUserWidget_Select.h"
#include "Global.h"
#include "Components/GridPanel.h"
#include "Components/Border.h"
#include "CUserWidget_SelectItem.h"

void UCUserWidget_Select::NativeConstruct()
{
	UGridPanel* gridPanel = Cast<UGridPanel>(GetWidgetFromName("Grid"));
	TArray<UWidget*> widgets = gridPanel->GetAllChildren();
	for (UWidget* widget : widgets)
	{
		FUserWidget_Select_Data data;
		data.Item = Cast<UCUserWidget_SelectItem>(widget);

		Datas.Add(data);
	}

	Super::NativeConstruct();
}

void UCUserWidget_Select::Clicked(FString InName)
{
	for (const FUserWidget_Select_Data& data : Datas)
	{
		if (data.Item->GetName() == InName)
		{
			if (data.OnUserWidget_Select_Clicked.IsBound())
				data.OnUserWidget_Select_Clicked.Broadcast();

			SetVisibility(ESlateVisibility::Hidden);
			UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = false;
			UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(FInputModeGameOnly());
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);

			break;
		}
	}
}

void UCUserWidget_Select::Hovered(FString InName)
{
	for (const FUserWidget_Select_Data& data : Datas)
	{
		UBorder* border = Cast<UBorder>(data.Item->GetWidgetFromName("BG_Border"));

		if (data.Item->GetName() == InName)
		{
			border->SetBrushColor(FLinearColor::Red);

			break;
		}
	}
}

void UCUserWidget_Select::Unhovered(FString InName)
{
	for (const FUserWidget_Select_Data& data : Datas)
	{
		UBorder* border = Cast<UBorder>(data.Item->GetWidgetFromName("BG_Border"));

		if (data.Item->GetName() == InName)
		{
			border->SetBrushColor(FLinearColor::White);

			break;
		}
	}
}
