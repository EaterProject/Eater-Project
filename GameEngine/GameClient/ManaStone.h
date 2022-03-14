#pragma once
#include "Component.h"
class MeshFilter;
class Transform;
class GameObject;
class AnimationController;

class ManaStone :public Component
{
public:
	ManaStone();
	virtual ~ManaStone();
	void Awake();
	void SetUp();
	void Update();
	void ManaStoneUpdate(Vector3 Pos);
private:
	MeshFilter*				mMeshFilter;
	Transform*				mTransform;
	AnimationController*	mAnimation;
};

