#pragma once
#include <Component.h>
class Collider;
class MeshFilter;
class Transform;
class Image;
class Store :public Component
{
public:
	Store();
	virtual ~Store();

	void Awake() override;
	void SetUp() override;
	void Update() override;
	void Debug() override;

	void StoreActive(int Type);

	void OnTriggerEnter(GameObject* Obj);
	void OnTriggerExit(GameObject* Obj);

private:
	Collider*	mCollider	= nullptr;
	MeshFilter* mMeshFilter = nullptr;
	Transform*	mTransform	= nullptr;

	Image* mStoreActive = nullptr;

	bool IsRange = false;

	int PlayerTag = 0;
};

