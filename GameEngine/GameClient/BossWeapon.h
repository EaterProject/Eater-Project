#pragma once
#include <Component.h>
#include "MeshFilterSetting.h"
class MeshFilter;
class Transform;
class BossWeapon :public Component
{
public:
	BossWeapon();
	virtual ~BossWeapon();

	void Awake() override;
	void SetUp() override;
	void Update() override;


	void SetShootingPoistion(Vector3 Start,Vector3 End);
	void Reset();
	bool ShootingReady();
private:
	void RandomUpdate();
	
	MeshFilter* mMeshFilter = nullptr;
	Transform*	mTransform	= nullptr;
	float Speed			= 20.0f;
	bool IsShooting		= false;
	bool IsSizeUpdate	= false;
	bool IsReady		= false;
	bool IsStart		= false;

	Vector3 Start_Shooting_Pos;
	Vector3 End_Shooting_Pos;
	Vector3 ShootingNormalize;

	MeshFilterSetting mMF_Setting;
};

