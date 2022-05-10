#pragma once
#include "ClientComponent.h"
#include <vector>
class MeshFilter;
class Transform;
class GameObject;
class AnimationController;
class MonsterA;

class ManaStone :public ClientComponent
{
public:
	ManaStone();
	~ManaStone();
	void Awake() override;
	void SetUp() override;
	void Update() override;
	
	Vector3 GetPoint(int CreateRangeIndex, int MovePointIndex);
private:
	MeshFilter* mMeshFilter;
	Transform*	mTransform;
	AnimationController* mAnimation;
private:
	Vector3 GetMonsterPoint(const Vector3& MyPosition, int MonsterPointIndex , float Range);
	void Debug();
	void CreateMonsterRangePoint(int MonsterCount);


	static std::vector<Vector3> MonsterMovePoint;
	std::vector<MonsterA*> MonsterList;

	float Range = 10;
};

