#pragma once
#include "Component.h"

class MeshFilter;
class Transform;
class GameObject;
class AnimationController;
class Monster :public Component
{
public:
	Monster();
	virtual ~Monster();
	void Awake();
	void SetUp();
	void Update();

	void MonsterUpdate(Vector3 Pos);
private:
	MeshFilter* mMeshFilter;
	Transform*	mTransform;
	AnimationController* mAnimation;


};

