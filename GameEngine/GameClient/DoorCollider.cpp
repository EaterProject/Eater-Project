#include "DoorCollider.h"

#include "EaterEngineAPI.h"
#include "MessageManager.h"


#include "GameObject.h"
#include "MeshFilter.h"
#include "Transform.h"
#include "Collider.h"
DoorCollider::DoorCollider()
{
}

DoorCollider::~DoorCollider()
{
}

void DoorCollider::Awake()
{
	mCollider = gameobject->GetComponent<Collider>();
	mTransform = gameobject->GetTransform();

	PlayerTag = FindTagNumber("Player");
}

void DoorCollider::SetUp()
{
	mCollider->SetSphereCollider(10);
	mCollider->SetTrigger(true);

	switch (GateNumber)
	{
	case 0:
		GroundCollider = FindGameObjectTag("Gate_00_Ground");
		gameobject->GetTransform()->SetPosition(-9, 0, 28);
		break;
	case 1:
		GroundCollider = FindGameObjectTag("Gate_01_Ground");
		gameobject->GetTransform()->SetPosition(24, 0, 17);
		break;
	case 2:
		GroundCollider = FindGameObjectTag("Gate_02_Ground");
		gameobject->GetTransform()->SetPosition(24, 0, -18);
		break;
	case 3:
		GroundCollider = FindGameObjectTag("Gate_03_Ground");
		gameobject->GetTransform()->SetPosition(-24, 0, -17);
		break;
	}

	GroundCollider->GetTransform()->SetPosition_Y(-10.0f);
}

void DoorCollider::Debug()
{
	DebugDrawCircle(10, mTransform->GetPosition(), Vector3(0,0,0),Vector3(1,0,0));
}

void DoorCollider::SetGateNumber(int num)
{
	GateNumber = num;
}

void DoorCollider::OnTriggerEnter(GameObject* Obj)
{
	if (Lock == false)
	{
		if (Obj->GetTag() == 0)
		{
			//범위 안에 들어왔다면  Player UI를 끄고 상점 UI를 켠다
			int num = 0;
			bool ON = true;
			MessageManager::GetGM()->SEND_Message(TARGET_GATE_MANAGER, MESSAGE_GATE_OPEN, &GateNumber);
			GroundCollider->GetTransform()->SetPosition_Y(-0.9f);
		}
	}
}

void DoorCollider::OnTriggerExit(GameObject* Obj)
{
	if (GateNumber == 0 && isStart == false)
	{
		int num = 1;
		MessageManager::GetGM()->SEND_Message(TARGET_GATE_MANAGER, MESSAGE_GATE_UNLOCK, &num);
		num++;
		MessageManager::GetGM()->SEND_Message(TARGET_GATE_MANAGER, MESSAGE_GATE_UNLOCK, &num);
		num++;
		MessageManager::GetGM()->SEND_Message(TARGET_GATE_MANAGER, MESSAGE_GATE_UNLOCK, &num);

		isStart = true;
	}

	if (Lock == false)
	{
		if (Obj->GetTag() == 0)
		{
			MessageManager::GetGM()->SEND_Message(TARGET_GATE_MANAGER, MESSAGE_GATE_CLOSE, &GateNumber);
			GroundCollider->GetTransform()->SetPosition_Y(-10.0f);
		}
	}
}
