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
	IsCreate = true;
}

void Drone::SetUp()
{
	mMeshFilter->SetModelName("drone");
	Vector3 DronePos = Vector3::Lerp(mTransform->GetPosition(), mPlayerTR->GetPosition(), GetDeltaTime());
	DronePos.y = OffsetY;
	mTransform->SetPosition(DronePos);
}

void Drone::Update()
{	
	if (IsCreate == false) { return; }

	Vector3 position = mPlayerTR->GetPosition();

	if (mTransform->GetDistance(position) >= 3.0f)
	{
		Vector3 DronePos = Vector3::Lerp(mTransform->GetPosition(), position, GetDeltaTime());
		DronePos.y = OffsetY + position.y;
		mTransform->SetPosition(DronePos);
	}

	//LookAt
	mTransform->Slow_Y_Rotation(position, 100, true);
}




