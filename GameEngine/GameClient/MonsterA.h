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
	void OnTriggerStay(GameObject* Obj);
private:
	MeshFilter*				mMeshFilter;
	Transform*				mTransform;
	AnimationController*	mAnimation;
	Collider*				mColider;
	Rigidbody*				mRigidbody;
	MonsterBase*			mBase;
private:
	float BackAttackSlipPower; //공격당했을떄 뒤로 밀려나는 힘 

};

