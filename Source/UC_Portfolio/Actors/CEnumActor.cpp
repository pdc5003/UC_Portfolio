#include "Global.h"
#include "Kismet/KismetStringLibrary.h"
#include "Actors/CEnumActor.h"

ACEnumActor::ACEnumActor()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ACEnumActor::BeginPlay()
{
	Super::BeginPlay();
	
	PrintToEnum();
}

void ACEnumActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACEnumActor::PrintToEnum()
{
	//�ܼ� for���� Max_Graphic���� ���� ���� enum->int�� ĳ����
	int last = static_cast<int>(EGraphicQualityType::Max_Graphic);

	//���̸� ���� �� �ʱ�ȭ
	FName rowName = L"";

	//void Ÿ���� ���ٽ�
	//���ٽ��� �� ������ �ϴ� �׷��� ��û ���⽺���� �Լ��� �ƴϸ�, ���� ū ������
	// �ڱⰡ ���� Ŭ������ �ƴ϶� ��� �κп� �Լ��� �Ӹ��� �����
	// cpp�� ������ ���Ǹ� �ϱⰡ ���׷��� �����̴�....
	//�Ű������� rowName�� ������ �ް�, IsGraphicQuality�� ���� ���� �����ϰų� ��������� �������� const������ �޴´�.
	//Ÿ���� auto�� �� �Ŵ� �Ʒ� for���� ������ �����̱⵵ �ϰ� ���� ���������� 
	//auto�� ���� �װ� �ƴ� ��
	/*
	* [](FName& RowName, const int& IsGraphicQuality)->void { ���� �����...}(FName& RowName, const int& IsGraphicQuality);
	*/
	//���Ͱ��� ��°�� ��ߵȴ�. ���� �Լ� ������ó�� EnumToName�� ���ٽ��� �ְ� EnumToName(FName& RowName, const int& IsGraphicQuality)��
	//���������� �������� �����̰� �ξ� ���� ���ϴ�...
	auto EnumToName = [](FName& RowName, const int& IsGraphicQuality)->void
	{
		//FString �ӽú��� FName���� Replace()�� �Լ��� ��� �ϴ� FString���� �޴´�.
		FString str = "";
		
		//static_cast<EGraphicQualityType>(IsGraphicQuality) �̰� �ܼ��� GetValueAsString�Լ��� �־ �Ǳ��ϴµ�
		//�� �� �����ϰ� �� �� �ִ� ����̶�� �Ǿ��־ TEnumAsByte�� �����.
		TEnumAsByte<EGraphicQualityType> e = static_cast<EGraphicQualityType>(IsGraphicQuality);
		
		//enum�� FString���� �ٲ��ش� ������ EGraphicQualityType::Default_Graphic�� ����� �ǹ����� ������
		//Replace�Լ��� ���� EGraphicQualityType::�� �κи� �����ش�. �� �� ����� ����� ���� �ϰ�
		//��� ã�ƺôµ� �̷��� �� ���Ѱ� �ȳ���...
		UEnum::GetValueAsString<EGraphicQualityType>(e, str);
		str = str.Replace(L"EGraphicQualityType::", L"");
		
		//�𸮾� TCHAR�� FName�̶� ȣȯ�����ϹǷ� *�ٿ��� ����
		RowName = *str;
	};

	for (int i = 0; i <= last; i++)
	{
		rowName = L"";
		EnumToName(rowName, i);
		CLog::Print(rowName.ToString(), i + 1, 3.0f);
	}


}

