#pragma once
#include "Component.h"

/// <summary>
/// �÷��̾ ����ٴϴ� ���� ���
/// </summary>
class Transform;
class GameObject;
class Player;
class HealingDrone : public Component
{
public:
	HealingDrone();
	virtual ~HealingDrone();
	void SetUp();
	void Update();
	void SetPlayer(GameObject* PlayerObject);
private:
	Transform*	mTransform;
	Transform*	mPlayerTR;
	GameObject* mPlayer;
	Player*		mPlayerComponent;
private:
	float HealingTime = 0;
private:
	const float DistanceOffset	= 4.0f; //�÷��̾� �Ÿ� ������
	const float OffsetY			= 3.5f;	//�����ġ ������
	const float HealingMaxTime	= 5.0f;	//�� ��Ÿ��
	const float HealingPower	= 5.0f;	//����
};

