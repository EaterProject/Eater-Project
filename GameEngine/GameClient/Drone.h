#pragma once
#include "ClientComponent.h"

/// <summary>
/// 플레이어를 따라다니는 힐링 드론
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
	const float DistanceOffset	= 4.0f; //플레이어 거리 오프셋
	const float OffsetY			= 1.5f;	//드론위치 오프셋
	float FollowSpeed = 2.0f;			//플레이어 쫒아가는 속도
	bool IsCreate = false;
};

