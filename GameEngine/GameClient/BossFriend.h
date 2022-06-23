#pragma once
#include <Component.h>
class MeshFilter;
class Transform;
class AnimationController;
class BossFriend :public Component
{
public:
	BossFriend();
	virtual~BossFriend();

	void Awake() override;
	void SetUp() override;
	void Update() override;

	void SetPosition(Vector3 Pos);
	void SetPosition(float x, float y,float z);
private:
	Transform*	mTransform = nullptr;
	MeshFilter* mMeshFilter = nullptr;
	AnimationController* mAnimation = nullptr;
};

