#pragma once
class GameObject;
class Transform;
class Rigidbody;
class Collider;
class AnimationController;

class BossStateManager
{
public:
	BossStateManager();
	~BossStateManager();

	void ObjectSetting(GameObject* BossObject);

	void Idle();
	void DEAD();
	void Groggy_Start();
	void Groggy_Play();
	void Groggy_End();
	void Teleport_Ready();
	void Teleport_Start();
	void Create();
	void Closer_Attack_L();
	void Closer_Attack_R();

	void Boss_Chase_Attack_Ready();
	void Boss_Chase_Attack_Play();
	void Boss_Chase_Attack_End();

	void Boss_Rendom_Attack_Start();
	void Boss_Rendom_Attack_Play();
	void Boss_Rendom_Attack_End();
	void Boss_Rendom_Attack_Reset();

	void Boss_Hit();
private:
	//ÄÄÆ÷³ÍÆ®
	AnimationController*	mAnime		= nullptr;
	Transform*				mTransform	= nullptr;
	Rigidbody*				mRigidbody	= nullptr;
	Collider*				mCollider	= nullptr;
private:
	//Look Object
	GameObject*				LookObj		= nullptr;
	Transform*				LookObjTR	= nullptr;
private:

};

