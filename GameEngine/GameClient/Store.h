#pragma once
#include <Component.h>
class Collider;
class MeshFilter;
class Transform;
class Store :public Component
{
public:
	Store();
	virtual ~Store();

	void Awake() override;
	void SetUp() override;
	void Debug() override;

	void OnTriggerEnter(GameObject* Obj);
	void OnTriggerExit(GameObject* Obj);
private:
	Collider*	mCollider	= nullptr;
	MeshFilter* mMeshFilter = nullptr;
	Transform*	mTransform	= nullptr;

	int PlayerTag = 0;
};

