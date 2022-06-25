#pragma once
#include "ClientComponent.h"

/// <summary>
/// �÷��̾ ����ٴϴ� ���� ���
/// </summary>
class Transform;
class GameObject;
class Player;
class MeshFilter;
class Drone : public ClientComponent
{
public:
	Drone();
	virtual ~Drone();
	void Awake();
	void SetUp();
	void Update();
private:
	Transform*	mTransform;
	Transform*	mPlayerTR;
	GameObject* mPlayer;
	Player*		mPlayerComponent;
	MeshFilter* mMeshFilter;
private:
	const float DistanceOffset	= 4.0f; //�÷��̾� �Ÿ� ������
	const float OffsetY			= 1.5f;	//�����ġ ������
	float FollowSpeed = 2.0f;			//�÷��̾� �i�ư��� �ӵ�
	bool IsCreate = false;
};

