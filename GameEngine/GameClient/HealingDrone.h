#pragma once
#include "ClientComponent.h"

/// <summary>
/// �÷��̾ ����ٴϴ� ���� ���
/// </summary>
class Transform;
class GameObject;
class Player;
class MeshFilter;
class HealingDrone : public ClientComponent
{
public:
	HealingDrone();
	virtual ~HealingDrone();
	void Awake();
	void SetUp();
	void Update();
	void ReSet();
	void SetPlayer(GameObject* PlayerObject);
private:
	Transform*	mTransform;
	Transform*	mPlayerTR;
	GameObject* mPlayer;
	Player*		mPlayerComponent;
	MeshFilter* mMeshFilter;
private:
	float HealingTime = 0;
private:
	const float DistanceOffset	= 4.0f; //�÷��̾� �Ÿ� ������
	const float OffsetY			= 3.5f;	//�����ġ ������
	const float HealingMaxTime	= 5.0f;	//�� ��Ÿ��
	const float HealingPower	= 5.0f;	//����
};

