#include "AngleActor.h"
#include "Global.h"
// Sets default values
AAngleActor::AAngleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAngleActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAngleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Ball == nullptr || Wind == nullptr)
		return;

	CalAngle();
	CalAngle2D();
}

void AAngleActor::CalAngle()
{
	FVector start = FVector(Ball->GetActorLocation().X, Ball->GetActorLocation().Y, 0);
	FVector dest = FVector(Wind->GetActorLocation().X, Wind->GetActorLocation().Y, 0);

	FVector dir = dest - start;
	dir = FVector(dir.X, dir.Y, 0.0f);
	FVector dirNormal = dir.GetSafeNormal();

	float dot = FVector::DotProduct(FVector::ForwardVector, dirNormal);
	float AcosAngle = FMath::Acos(dot);
	float angle = FMath::RadiansToDegrees(AcosAngle);

	FVector cross = FVector::CrossProduct(FVector::ForwardVector, dirNormal);

	float rotAngle = 0.0f;

	if (cross.Z < 0)
		rotAngle = 360.0f - angle;
	else
		rotAngle = angle;

	CLog::Print(rotAngle, 2, 3.0f, FColor::Red);
}

void AAngleActor::CalAngle2D()
{
	//골프공 벡터2D
	FVector2D ball = FVector2D(Ball->GetActorLocation().X, Ball->GetActorLocation().Y);
	//바람 벡터2D
	FVector2D wind = FVector2D(Wind->GetActorLocation().X, Wind->GetActorLocation().Y);

	//바람이 부는 방향
	FVector2D dir = wind - ball;
	//바람이 부는 방향 노멀라이즈
	FVector2D dirNormal = dir.GetSafeNormal();

	//골프공과 바람 부는 방향 내적
	float dot = FVector2D::DotProduct(ball.GetSafeNormal(), dirNormal);
	//아크코사인
	float AcosAngle = FMath::Acos(dot);
	//각도로 바꿈
	float angle = FMath::RadiansToDegrees(AcosAngle);
	//외적(좌우 구분)
	float cross = FVector2D::CrossProduct(ball.GetSafeNormal(), dirNormal);

	float rotAngle = 0.0f;

	if (cross < 0)
		rotAngle = 360.0f - angle;
	else
		rotAngle = angle;

	//최대한 2D벡터로 처리했으며 360도로 표기되게함
	//의심(?)되는 것 중 하나가 UI의 바람 부는 방향을 
	//어디를 기준으로 바람이 부는 방향과 각도를 재는지가 의문이듬
	//코드상으론 공을 기준으로 했는데도 불구하고 오차가 난다면...
	//정말 잘 모르겠음
}



