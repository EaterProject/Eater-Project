#include "DoorCollider.h"

#include "EaterEngineAPI.h"
#include "MessageManager.h"


#include "GameObject.h"
#include "MeshFilter.h"
#include "Transform.h"
#include "Collider.h"

bool DoorCollider::Boss_Start = false;

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
			//���� �ȿ� ���Դٸ�  Player UI�� ���� ���� UI�� �Ҵ�
			int num = 0;
			bool ON = true;
			MessageManager::GetGM()->SEND_Message(TARGET_GATE_MANAGER, MESSAGE_GATE_OPEN, &GateNumber);
			GroundCollider->GetTransform()->SetPosition_Y(-0.99f);


			PlayerDistance = Vector3::Distance(Zero, Obj->transform->GetPosition());
			if (PlayerDistance >= 30)
			{
				Sound_Stop_BGM();
				Sound_Play_BGM("InGame_OutDoor");
			}
			else
			{
				Sound_Stop_BGM();
				Sound_Play_BGM("InGame_InDoor");
			}
		}
	}
}

void DoorCollider::OnTriggerStay(GameObject* Obj)
{
	if (Obj->GetTag() == 0)
	{
		PlayerDistance = Vector3::Distance(Zero, Obj->transform->GetPosition());

		if (PlayerDistance >= 25.0f && PlayerDistance <= 30.0f)
		{
			BlendFactor = (30.0f - PlayerDistance) * 0.2f;
			SetColorGradingBlendFactor(BlendFactor);
		}

		if (PlayerDistance > 30.0f)
		{
			if (PlayerLightIndex != 0)
			{
				PlayerLightIndex = 0;
				MessageManager::GetGM()->SEND_Message(TARGET_PLAYER, MESSAGE_PLAYER_LIGHT_CHANGE, &PlayerLightIndex);
			}
		}
		else
		{
			if (PlayerLightIndex != 1)
			{
				PlayerLightIndex = 1;
				MessageManager::GetGM()->SEND_Message(TARGET_PLAYER, MESSAGE_PLAYER_LIGHT_CHANGE, &PlayerLightIndex);
			}
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

		if (FirstOut == false)
		{
			MessageManager::GetGM()->SEND_Message(TARGET_DRONE, MESSAGE_DRONE_DOOM_OUT);
			FirstOut = true;
		}

		isStart = true;
	}


	PlayerDistance = Vector3::Distance(Zero, Obj->transform->GetPosition());
	if (PlayerDistance >= 30)
	{
		Sound_Stop_BGM();
		if (Boss_Start)
		{
			Sound_Play_BGM("BossZone");
		}
		else
		{
			Sound_Play_BGM("InGame_OutDoor");
		}
	}
	else
	{
		Sound_Stop_BGM();
		Sound_Play_BGM("InGame_InDoor");
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