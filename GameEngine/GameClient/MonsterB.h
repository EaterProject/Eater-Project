#pragma once
#include "Component.h"
class MeshFilter;
class Transform;
class GameObject;
class AnimationController;
class Collider;
class Rigidbody;
class MonsterBase;

class MonsterB  :public Component
{
public:
	MonsterB();
	virtual ~MonsterB();
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

