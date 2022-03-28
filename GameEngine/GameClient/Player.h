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
	void Start();
	void Update();
	void StartUpdate();

	Transform* mTransform;
	float Speed =0;
private:
	//�÷��̾� Ű��ǲ
	void PlayerKeyinput();
private:
	///���۳�Ʈ
	AnimationController* mAnimation;
	Collider*		mCollider;
	MeshFilter*		mMeshFilter;
	Transform*		mCameraTR;
	Rigidbody*		mRigidbody;
	GameObject*		MeshObject;
private:
	Vector3 DirPos;			//����
	Vector3 DirRot;			//ȸ��
	Vector3 PastDirRot;		//������ ����

	float RotationDir;
};