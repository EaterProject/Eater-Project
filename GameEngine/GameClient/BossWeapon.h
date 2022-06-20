#pragma once
#include <Component.h>
class MeshFilter;
class Transform;
class BossWeapon :public Component
{
public:
	BossWeapon();
	virtual ~BossWeapon();

	void Awake() override;
	void SetUp() override;

private:
	MeshFilter* mMeshFilter = nullptr;
	Transform*	mTransform	= nullptr;
};

