#include "CUserWidget_SelectItem.h"
#include "Global.h"
#include "CUserWidget_Select.h"
#include "Characters/PPlayer.h"

void UCUserWidget_SelectItem::Click()
{
    GetSelectWidget()->Clicked(GetName());
}

void UCUserWidget_SelectItem::Hover()
{
    GetSelectWidget()->Hovered(GetName());
}

void UCUserWidget_SelectItem::Unhover()
{
    GetSelectWidget()->Unhovered(GetName());
}

UCUserWidget_Select* UCUserWidget_SelectItem::GetSelectWidget()
{
    //ACPlayer* player = Cast<ACPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    return nullptr;
}