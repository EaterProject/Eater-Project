#pragma once
#include "ClientComponent.h"
#include <vector>
class MeshFilter;
class Transform;
class GameObject;
class AnimationController;

class ManaStone :public ClientComponent
{
public:
	ManaStone();
	~ManaStone();
	void Awake() override;
	void SetUp() override;
	void Update() override;
	void ManaStoneUpdate(Vector3 Pos);
	// ClientComponent을(를) 통해 상속됨
	void ReSet() override;

	void CreateMonsterRangePoint(int MonsterCount);
private:
	MeshFilter*				mMeshFilter;
	Transform*				mTransform;
	AnimationController*	mAnimation;

	std::vector<Vector3> TrianglePoint;
	std::vector<Vector3> TriangleCenterPoint;
	float TrianglePointDir = 0;
};

