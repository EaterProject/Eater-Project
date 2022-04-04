#include "HealingDrone.h"
#include "GameObject.h"
#include "Transform.h"
#include "MainHeader.h"
#include "Player.h"
HealingDrone::HealingDrone()
{
	mTransform			= nullptr;
	mPlayerTR			= nullptr;
	mPlayer				= nullptr;
	mPlayerComponent	= nullptr;
}

HealingDrone::~HealingDrone()
{
	mTransform			= nullptr;
	mPlayerTR			= nullptr;
	mPlayer				= nullptr;
	mPlayerComponent	= nullptr;
}

void HealingDrone::SetUp()
{
	mTransform  = gameobject->GetTransform();
	mPlayerTR	= mPlayer->GetTransform();
	mPlayerComponent = mPlayer->GetComponent<Player>();

	Vector3 DronePos = Vector3::Lerp(mTransform->Position, mPlayerTR->Position, GetDeltaTime());
	DronePos.y = OffsetY;
	mTransform->Position = DronePos;
}

void HealingDrone::Update()
{	
	//Vector3 Test = mPlayerTR->Position;
	//Vector3 Right = mTransform->GetLocalPosition_Right();
	//Test +=  Right;
	////거리계산
	//
	//mTransform->Position = Vector3::Lerp(mTransform->Position, Test, 0.01f);
	//mTransform->Position.y = OffsetY;
	if (mTransform->GetDistance(mPlayerTR->Position) >= 4.0f) 
	{
		Vector3 DronePos = Vector3::Lerp(mTransform->Position, mPlayerTR->Position,GetDeltaTime());
		DronePos.y = OffsetY;
		mTransform->Position = DronePos;
	}

	//LookAt
	mTransform->Slow_Y_Rotation(mPlayerTR->Position,100);

	//체력 회복
	HealingTime += GetDeltaTime();
	if (HealingTime >= HealingMaxTime)
	{
		mPlayerComponent->Healing(HealingPower);
		HealingTime -= HealingMaxTime;
	}
}

void HealingDrone::SetPlayer(GameObject* PlayerObject)
{
	mPlayer = PlayerObject;
}

