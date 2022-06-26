#pragma once
#include <Component.h>
class Collider;
class Rigidbody;
class Transform;
class PlayerCollider :public Component
{
public:
	PlayerCollider();
	virtual ~PlayerCollider();

	void Awake() override;
	void SetUp() override;

	void Update() override;
	void Debug() override;

	void OnTriggerEnter(GameObject* Obj) override;
	void OnTriggerExit(GameObject* Obj) override;
	void OnTriggerStay(GameObject* Obj) override;
private:
	Collider*	mCollider	= nullptr;
	Rigidbody*	mRigidbody	= nullptr;
	Transform*	mTransform	= nullptr;
	Transform*	mPlayerTR	= nullptr;

	int Gate_01_IN_Tag = 9;
	int Gate_01_OUT_Tag = 9;
};

