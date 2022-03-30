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

class Player :public Component
{
public:
	Player();
	virtual ~Player();
	void Awake();
	void SetUp();
	void Update();
	void StartUpdate();

	float Speed =0;
private:
	//�÷��̾� Ű��ǲ
	void PlayerKeyinput();
private:
	///���۳�Ʈ
	Transform* mTransform;
	AnimationController* mAnimation;
	Collider*		mCollider;
	MeshFilter*		mMeshFilter;
	Transform*		mCameraTR;
	Rigidbody*		mRigidbody;
private:
	Vector3 DirPos;			//����
	Vector3 DirRot;			//ȸ��
	Vector3 PastDirRot;		//������ ����


	GameObject* AttackColliderObject;
	Collider* AttackCollider;
	Rigidbody* AttackRigidbody;

	float RotationDir;
};