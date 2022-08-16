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
	//������ ����2D
	FVector2D ball = FVector2D(Ball->GetActorLocation().X, Ball->GetActorLocation().Y);
	//�ٶ� ����2D
	FVector2D wind = FVector2D(Wind->GetActorLocation().X, Wind->GetActorLocation().Y);

	//�ٶ��� �δ� ����
	FVector2D dir = wind - ball;
	//�ٶ��� �δ� ���� ��ֶ�����
	FVector2D dirNormal = dir.GetSafeNormal();

	//�������� �ٶ� �δ� ���� ����
	float dot = FVector2D::DotProduct(ball.GetSafeNormal(), dirNormal);
	//��ũ�ڻ���
	float AcosAngle = FMath::Acos(dot);
	//������ �ٲ�
	float angle = FMath::RadiansToDegrees(AcosAngle);
	//����(�¿� ����)
	float cross = FVector2D::CrossProduct(ball.GetSafeNormal(), dirNormal);

	float rotAngle = 0.0f;

	if (cross < 0)
		rotAngle = 360.0f - angle;
	else
		rotAngle = angle;

	//�ִ��� 2D���ͷ� ó�������� 360���� ǥ��ǰ���
	//�ǽ�(?)�Ǵ� �� �� �ϳ��� UI�� �ٶ� �δ� ������ 
	//��� �������� �ٶ��� �δ� ����� ������ ������� �ǹ��̵�
	//�ڵ������ ���� �������� �ߴµ��� �ұ��ϰ� ������ ���ٸ�...
	//���� �� �𸣰���
}



