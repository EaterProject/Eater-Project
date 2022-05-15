#pragma once
#include "NetworkComponent.h"
#include <queue>
#include "PlayerData.h"

class AnimationController;
class Transform;
class MeshFilter;
class Camera;
class Rigidbody;
class FrameData;
class Collider;
class PhysRayCast;
enum class PLAYER_STATE
{
	IDLE,
	ATTACK,
	DEAD,
};
class Player :public Component
{
public:
	Player();
	virtual ~Player();
	void Awake() override;
	void Start() override;
	void Update() override;
	
	static Transform* GetPlayerTransform();
	static PLAYER_STATE GetState();

	void Healing(float HealingPower);
private:
	//�÷��̾� Ű��ǲ
	void PlayerKeyinput();
	void PlayerStateUpdate();
	void PlayerAttackColliderUpdate();
	void PlayerGroundCheck();
private:
	///���۳�Ʈ
	static Transform* mTransform;
	AnimationController* mAnimation;
	MeshFilter*		mMeshFilter;
	Transform*		mCameraTR;
private:
	Vector3 DirPos;			//����
	Vector3 DirRot;			//ȸ��
	Vector3 PastDirRot;		//������ ����

	GameObject* AttackColliderObject;
	Collider*	AttackCollider;


	float Speed = 10;
	float HP	= 100;
	static PLAYER_STATE mState;
private:
	PhysRayCast* RayCastHit;
	const int Ray_Front = 0;
	const int Ray_Back	= 1;
	const int Ray_Right = 2;
	const int Ray_Left	= 3;
	const int Ray_Center= 4;
	bool IsFrontRayCheck = false;
	bool IsBackRayCheck = false;
	bool IsRightRayCheck = false;
	bool IsLeftRayCheck = false;

	int HitCount = 0;
};