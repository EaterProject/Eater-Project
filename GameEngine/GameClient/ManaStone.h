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
	void Debug() override;
	
	void SetMonsterCount(int MonsterA, int MonsterB);
	Vector3 GetPoint(int CreateRangeIndex, int MovePointIndex);
	void CreateMonster(int MonsterACount, int MonsterBCount);
private:
	MeshFilter* mMeshFilter;
	Transform*	mTransform;
	AnimationController* mAnimation;
private:
	void CreateMonsterA(int index);
	void CreateMonsterB(int index);

	Vector3 GetMonsterPoint(const Vector3& MyPosition, int MonsterPointIndex , float Range);
	void CreateMonsterRangePoint(int MonsterCount);


	static std::vector<Vector3> MonsterMovePointDefault;
	Vector3 MovePoint[5];
	std::vector<MonsterA*> MonsterA_List;
	std::vector<MonsterB*> MonsterB_List;

	float Range = 10;
	int MonsterACount = 0;
	int MonsterBCount = 0;
};

