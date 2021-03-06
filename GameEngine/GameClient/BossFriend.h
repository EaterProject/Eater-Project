#pragma once
#include "MeshFilterSetting.h"
#include <Component.h>
class MeshFilter;
class Transform;
class AnimationController;
class MeshFilterSetting;
class BossFriend :public Component
{
public:
	BossFriend();
	virtual~BossFriend();

	void Awake() override;
	void SetUp() override;
	void Start() override;
	void Update() override;
	
	void PlayDissolve(float playtime, DISSOLVE_OPTION option);
	void SetPosition(Vector3 Pos);
	void SetPosition(float x, float y,float z);
	void Reset();
private:
	Transform*	mTransform = nullptr;
	MeshFilter* mMeshFilter = nullptr;
	AnimationController* mAnimation = nullptr;
	MeshFilterSetting mSetting;
	Vector3 StartPos;
};

