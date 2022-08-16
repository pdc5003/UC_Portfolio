#include "CAIController.h"
#include "Global.h"
#include "CEnemy_AI.h"
#include "PPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/CBehaviorComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

ACAIController::ACAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateActorComponent<UBlackboardComponent>(this, &Blackboard, "Blackboard");
	CHelpers::CreateActorComponent<UCBehaviorComponent>(this, &Behavior, "Behavior");
	CHelpers::CreateActorComponent<UAIPerceptionComponent>(this, &Perception, "Perception");

	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");
	Sight->SightRadius = 600.0f;
	Sight->LoseSightRadius = 800.0f;
	Sight->PeripheralVisionAngleDegrees = 90.0f;
	Sight->SetMaxAge(2.0f);

	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectFriendlies = false;
	Sight->DetectionByAffiliation.bDetectNeutrals = false;

	Perception->ConfigureSense(*Sight);
	Perception->SetDominantSense(*Sight->GetSenseImplementation());
}

float ACAIController::GetSightRadius()
{
	return Sight->SightRadius;
}

void ACAIController::BeginPlay()
{
	Super::BeginPlay();

}

void ACAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (bDrawDebug)
	{
		FVector center = OwnerEnemy->GetActorLocation();
		center.Z -= AdjustCircleHeight;
		DrawDebugCircle(GetWorld(), center, Sight->SightRadius, 300, FColor::Green, false, -1.0f, 0, 0, FVector::RightVector, FVector::ForwardVector);
		DrawDebugCircle(GetWorld(), center, ActionRange, 300, FColor::Red, false, -1.0f, 0, 0, FVector::RightVector, FVector::ForwardVector);
		DrawDebugCircle(GetWorld(), center, MiddleRange, 300, FColor::Blue, false, -1.0f, 0, 0, FVector::RightVector, FVector::ForwardVector);
	}
}

void ACAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	OwnerEnemy = Cast<ACEnemy_AI>(InPawn);
	SetGenericTeamId(OwnerEnemy->GetTeamID());
	
	Perception->OnPerceptionUpdated.AddDynamic(this, &ACAIController::OnPerceptionUpdated);

	
	UseBlackboard(OwnerEnemy->GetBehaviorTree()->BlackboardAsset, Blackboard);
	Behavior->SetBlackboard(Blackboard);
	
	RunBehaviorTree(OwnerEnemy->GetBehaviorTree());
	
}

void ACAIController::OnUnPossess()
{
	Super::OnUnPossess();
	
	Perception->OnPerceptionUpdated.Clear();
}

void ACAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	TArray<AActor*> actors;
	Perception->GetCurrentlyPerceivedActors(NULL, actors);

	APPlayer* player = NULL;
	for (AActor* actor : actors)
	{
		player = Cast<APPlayer>(actor);

		if (!!player)
			break;
			
	}

	targetPlayer = player;

	Blackboard->SetValueAsObject("Player", player);
}

