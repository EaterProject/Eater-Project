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
}

void HealingDrone::Update()
{	
	//거리계산
	if (mTransform->GetDistance(mPlayerTR->Position) >= 4.0f) 
	{
		mTransform->Position = Vector3::Lerp(mTransform->Position, mPlayerTR->Position,GetDeltaTime()*0.75f);
		mTransform->Position.y = OffsetY;
	}

	//LookAt
	Vector3 PlayerVec = mPlayerTR->Position;
	//PlayerVec.z *= -1;
	mTransform->Slow_Y_Rotation(PlayerVec,50);

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

