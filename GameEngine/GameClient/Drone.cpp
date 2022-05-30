#include "Drone.h"
#include "GameObject.h"
#include "Transform.h"
#include "EaterEngineAPI.h"
#include "Player.h"
#include "MeshFilter.h"
Drone::Drone()
{
	mTransform			= nullptr;
	mPlayerTR			= nullptr;
	mPlayer				= nullptr;
	mPlayerComponent	= nullptr;
}

Drone::~Drone()
{
	mTransform			= nullptr;
	mPlayerTR			= nullptr;
	mPlayer				= nullptr;
	mPlayerComponent	= nullptr;
}

void Drone::Awake()
{
	mPlayer				= FindGameObjectTag("Player");
	mPlayerComponent	= mPlayer->GetComponent<Player>();
	mPlayerTR			= mPlayer->GetTransform();


	mTransform			= gameobject->GetTransform();
	mMeshFilter			= gameobject->GetComponent<MeshFilter>();
}

void Drone::SetUp()
{
	mMeshFilter->SetModelName("drone");
	Vector3 DronePos = Vector3::Lerp(mTransform->Position, mPlayerTR->Position, GetDeltaTime());
	DronePos.y = OffsetY;
	mTransform->Position = DronePos;
}

void Drone::Update()
{	
	if (mTransform->GetDistance(mPlayerTR->Position) >= 3.0f) 
	{
		Vector3 DronePos = Vector3::Lerp(mTransform->Position, mPlayerTR->Position,GetDeltaTime());
		DronePos.y = OffsetY + mPlayerTR->Position.y;
		mTransform->Position = DronePos;
	}

	//LookAt
	mTransform->Slow_Y_Rotation(mPlayerTR->Position,50,true);
}




