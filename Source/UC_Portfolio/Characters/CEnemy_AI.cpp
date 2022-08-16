#include "CEnemy_AI.h"
#include "Global.h"
#include "Components/CPatrolComponent.h"

ACEnemy_AI::ACEnemy_AI()
{
}

FGenericTeamId ACEnemy_AI::GetGenericTeamId() const
{
	return TeamID;
}
