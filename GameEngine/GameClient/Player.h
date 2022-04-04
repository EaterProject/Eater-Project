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
class PlayerData;
class Collider;
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
	void Awake();
	void SetUp();
	void Update();
	void StartUpdate();
	
	static Transform* GetPlayerTransform();
	static PLAYER_STATE GetState();

	void Healing(float HealingPower);
private:
	//플레이어 키인풋
	void PlayerKeyinput();
private:
	///컨퍼넌트
	static Transform* mTransform;
	AnimationController* mAnimation;
	Collider*		mCollider;
	MeshFilter*		mMeshFilter;
	Transform*		mCameraTR;
	Rigidbody*		mRigidbody;
private:
	Vector3 DirPos;			//방향
	Vector3 DirRot;			//회전
	Vector3 PastDirRot;		//과거의 방향

	GameObject* AttackColliderObject;
	Collider*	AttackCollider;
	Rigidbody*	AttackRigidbody;

	float Speed = 0;
	float HP	= 100;
	float RotationDir;
	static PLAYER_STATE mState;
};