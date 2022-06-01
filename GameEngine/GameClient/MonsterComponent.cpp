#include "MonsterComponent.h"
#include "GameObject.h"
#include "Transform.h"

void MonsterComponent::SetMovePoint(float x, float y, float z)
{
	DirPoint = (gameobject->GetTransform()->Position - Vector3(x, y, z)) * -1;
	DirPoint.Normalize();
	DirPoint *= Speed;
	MovePoint.x = x;
	MovePoint.y = 1;
	MovePoint.z = z;
	ReturnPoint = MovePoint;
}

bool MonsterComponent::GetStopPoint()
{
	Transform* mTransform = gameobject->GetTransform();
	if (mTransform->Position.x > (MovePoint.x - 0.5f) && mTransform->Position.x < (MovePoint.x + 0.5f) &&
		mTransform->Position.z >(MovePoint.z - 0.5f) && mTransform->Position.z < (MovePoint.z + 0.5f))
	{
		return true;

	}
	else
	{
		return false;
	}
}
