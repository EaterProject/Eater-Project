#pragma once
#include <Component.h>
#include "MeshFilterSetting.h"
class MeshFilter;
class Transform;
class GameObject;
class ParticleController;
class BossWeapon :public Component
{
public:
	BossWeapon();
	virtual ~BossWeapon();

	void Awake() override;
	void SetUp() override;
	void Start() override;
	void Update() override;
	void Debug() override;

	void SetScale(float UpSize);
	void SetShootingPoistion(Vector3 Start, Vector3 End, float mScaleSpeed = 5.0f, float mMoveSpeed = 20.0f,bool GroundExplosion = false);
	void Reset();
	bool ShootingReady();
	void Explosion();
private:

	GameObject* mParticleObj[3] = { nullptr };
	GameObject* mParticleManagerObject = nullptr;
	ParticleController* mParticleManager = nullptr;


	MeshFilter* mMeshFilter = nullptr;
	Transform*	mTransform = nullptr;
	Transform*	mPlayerTr = nullptr;
	float MoveSpeed = 5.0f;
	float ScaleSpeed = 20.0f;
	float UpScale = 5.0f;
	bool IsShooting = false;
	bool IsSizeUpdate = false;
	bool IsReady = false;
	bool IsStart = false;
	bool IsGroundExplosion = false;

	Vector3 Start_Shooting_Pos;
	Vector3 End_Shooting_Pos;
	Vector3 ShootingNormalize;
	Vector3 StartPoint;

	MeshFilterSetting mMF_Setting;
};

