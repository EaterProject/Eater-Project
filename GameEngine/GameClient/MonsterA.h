#pragma once
#include "Component.h"
class MeshFilter;
class Transform;
class GameObject;
class AnimationController;
class Collider;
class Rigidbody;
class MonsterBase;

class MonsterA :public Component
{
public:
	MonsterA();
	virtual ~MonsterA();
public:
	void Awake();
	void SetUp();
	void Update();
private:
	MeshFilter*				mMeshFilter;
	Transform*				mTransform;
	AnimationController*	mAnimation;
	Collider*				mColider;
	Rigidbody*				mRigidbody;
	MonsterBase*			mBase;
};

