#pragma once
#include "ClientComponent.h"

/// <summary>
/// 플레이어를 따라다니는 힐링 드론
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
	const float DistanceOffset	= 4.0f; //플레이어 거리 오프셋
	const float OffsetY			= 3.5f;	//드론위치 오프셋
	const float HealingMaxTime	= 5.0f;	//힐 쿨타임
	const float HealingPower	= 5.0f;	//힐량
};

