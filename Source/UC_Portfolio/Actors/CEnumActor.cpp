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
	//단순 for문을 Max_Graphic까지 돌기 위해 enum->int로 캐스팅
	int last = static_cast<int>(EGraphicQualityType::Max_Graphic);

	//행이름 선언 및 초기화
	FName rowName = L"";

	//void 타입의 람다식
	//람다식을 쓴 이유는 일단 그렇게 엄청 복잡스러운 함수도 아니며, 제일 큰 문제는
	// 자기가 만든 클래스가 아니라 헤더 부분에 함수의 머리를 만들고
	// cpp에 별도로 정의를 하기가 껄그럽기 때문이다....
	//매개변수로 rowName를 참조로 받고, IsGraphicQuality는 딱히 값을 대입하거나 변경사항이 없음으로 const참조로 받는다.
	//타입을 auto로 한 거는 아래 for문을 돌리기 위함이기도 하고 공식 문서에서도 
	//auto를 쓰며 그게 아닐 시
	/*
	* [](FName& RowName, const int& IsGraphicQuality)->void { 각종 내용들...}(FName& RowName, const int& IsGraphicQuality);
	*/
	//위와같이 통째로 써야된다. 차라리 함수 포인터처럼 EnumToName에 람다식을 넣고 EnumToName(FName& RowName, const int& IsGraphicQuality)이
	//개인적으론 가독성도 물론이고 훨씬 보기 편하다...
	auto EnumToName = [](FName& RowName, const int& IsGraphicQuality)->void
	{
		//FString 임시변수 FName에는 Replace()이 함수가 없어서 일단 FString으로 받는다.
		FString str = "";
		
		//static_cast<EGraphicQualityType>(IsGraphicQuality) 이걸 단순히 GetValueAsString함수에 넣어도 되긴하는데
		//좀 더 안전하게 할 수 있는 방법이라고 되어있어서 TEnumAsByte를 사용함.
		TEnumAsByte<EGraphicQualityType> e = static_cast<EGraphicQualityType>(IsGraphicQuality);
		
		//enum을 FString으로 바꿔준다 문제는 EGraphicQualityType::Default_Graphic이 출력이 되버리기 때문에
		//Replace함수를 통해 EGraphicQualityType::이 부분만 날려준다. 좀 더 깔끔한 방법이 없나 하고
		//계속 찾아봤는데 이렇다 할 만한게 안나옴...
		UEnum::GetValueAsString<EGraphicQualityType>(e, str);
		str = str.Replace(L"EGraphicQualityType::", L"");
		
		//언리얼 TCHAR가 FName이랑 호환가능하므로 *붙여서 대입
		RowName = *str;
	};

	for (int i = 0; i <= last; i++)
	{
		rowName = L"";
		EnumToName(rowName, i);
		CLog::Print(rowName.ToString(), i + 1, 3.0f);
	}


}

