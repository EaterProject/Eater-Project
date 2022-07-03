#include "GateDoor.h"
#include "MessageManager.h"
#include "EaterEngineAPI.h"
#include "GameObject.h"
#include "Transform.h"
#include "Collider.h"
#include "DoorCollider.h"
GateDoor::GateDoor()
{
}

GateDoor::~GateDoor()
{
}

void GateDoor::Awake()
{
	
	Door[0] = MessageManager::GetGM()->CREATE_MESSAGE(TARGET_GATE_IN);
	Door[1] = MessageManager::GetGM()->CREATE_MESSAGE(TARGET_GATE_OUT);

	Door[2] = MessageManager::GetGM()->CREATE_MESSAGE(TARGET_GATE_IN);
	Door[3] = MessageManager::GetGM()->CREATE_MESSAGE(TARGET_GATE_OUT);

	Door[4] = MessageManager::GetGM()->CREATE_MESSAGE(TARGET_GATE_IN);
	Door[5] = MessageManager::GetGM()->CREATE_MESSAGE(TARGET_GATE_OUT);

	Door[6] = MessageManager::GetGM()->CREATE_MESSAGE(TARGET_GATE_IN);
	Door[7] = MessageManager::GetGM()->CREATE_MESSAGE(TARGET_GATE_OUT);


	for (int i = 0; i < 4; i++)
	{
		GateColliderObject[i] = Instance();
		GateColliderObject[i]->AddComponent<Collider>();
		GateCollider[i] = GateColliderObject[i]->AddComponent<DoorCollider>();
		GateCollider[i]->SetGateNumber(i);
	}

}

void GateDoor::Start()
{
	Door[0]->GetTransform()->SetRotate(0.0f, -108.0f, 0.0f);
	Door[1]->GetTransform()->SetRotate(0.0f, -108.0f, 0.0f);

	Door[2]->GetTransform()->SetRotate(0.0f, 324.0f, 0.0f);
	Door[3]->GetTransform()->SetRotate(0.0f, 324.0f, 0.0f);

	Door[4]->GetTransform()->SetRotate(0.0f, 36.0f, 0.0f);
	Door[5]->GetTransform()->SetRotate(0.0f, 36.0f, 0.0f);

	Door[6]->GetTransform()->SetRotate(0.0f, 144.0f, 0.0f);
	Door[7]->GetTransform()->SetRotate(0.0f, 144.0f, 0.0f);

	for (int i = 0; i < 8; i++)
	{
		DoorMesh[i].Setting(Door[i]);
		DoorMesh[i].SetEmissiveSetting( 1,0,0,4.4);
	}

	SetUnLock(0);
	SetLock(1);
	SetLock(2);
	SetLock(3);
}

void GateDoor::SetOpen(int Number)
{
	Sound_Play_SFX("Gate_Open");
	switch(Number)
	{
	case 0:
		Open(0, 1);
		break;
	case 1:
		Open(2, 3);
		break;
	case 2:
		Open(4, 5);
		break;
	case 3:
		Open(6, 7);
		break;
	}
}

void GateDoor::SetClose(int Number)
{
	Sound_Play_SFX("Gate_Close");
	switch (Number)
	{
	case 0:
		Close(0, 1);
		break;
	case 1:
		Close(2,3);
		break;
	case 2:
		Close(4, 5);
		break;
	case 3:
		Close(6, 7);
		break;
	}
}

void GateDoor::SetLock(int Number)
{
	if (Number >= 4) { return; }
	switch (Number)
	{
	case 0:
		DoorMesh[0].SetEmissiveSetting(MeshFilterSetting::COLOR_TYPE::RED, 2.9);
		DoorMesh[1].SetEmissiveSetting(MeshFilterSetting::COLOR_TYPE::RED, 2.9);
		break;
	case 1:
		DoorMesh[2].SetEmissiveSetting(MeshFilterSetting::COLOR_TYPE::RED, 2.9);
		DoorMesh[3].SetEmissiveSetting(MeshFilterSetting::COLOR_TYPE::RED, 2.9);
		break;
	case 2:
		DoorMesh[4].SetEmissiveSetting(MeshFilterSetting::COLOR_TYPE::RED, 2.9);
		DoorMesh[5].SetEmissiveSetting(MeshFilterSetting::COLOR_TYPE::RED, 2.9);
		break;
	case 3:
		DoorMesh[6].SetEmissiveSetting(MeshFilterSetting::COLOR_TYPE::RED, 2.9);
		DoorMesh[7].SetEmissiveSetting(MeshFilterSetting::COLOR_TYPE::RED, 2.9);
		break;
	}

	GateCollider[Number]->Lock = true;
}

void GateDoor::SetUnLock(int Number)
{
	if (Number >= 4) { return; }
	if (Number >= 4) { return; }
	switch (Number)
	{
	case 0:
		DoorMesh[0].SetEmissiveSetting(255, 167, 96, 2.1);
		DoorMesh[1].SetEmissiveSetting(255, 167, 96, 2.1);
		break;
	case 1:
		DoorMesh[2].SetEmissiveSetting(255, 167, 96, 2.1);
		DoorMesh[3].SetEmissiveSetting(255, 167, 96, 2.1);
		break;
	case 2:
		DoorMesh[4].SetEmissiveSetting(255, 167, 96, 2.1);
		DoorMesh[5].SetEmissiveSetting(255, 167, 96, 2.1);
		break;
	case 3:
		DoorMesh[6].SetEmissiveSetting(255,167,96, 2.1);
		DoorMesh[7].SetEmissiveSetting(255, 167, 96, 2.1);
		break;
	}
	GateCollider[Number]->Lock = false;
}

void GateDoor::Update()
{
	if (IS_OPEN == true)
	{
		OpenTime = GetDeltaTime()*4.0f;
		if (DoorTR[0] != nullptr && DoorTR[2] != nullptr)
		{
			DoorTR[0]->AddPosition_Y(OpenTime);
			DoorTR[2]->AddPosition_Y(OpenTime);
		}

		if (DoorTR[1] != nullptr && DoorTR[3] != nullptr)
		{
			DoorTR[1]->AddPosition_Y(-OpenTime);
			DoorTR[3]->AddPosition_Y(-OpenTime);
		}

		if (DoorTR[0]->GetPosition().y >= 4.0f)
		{
			DoorTR[0]->AddPosition_Y(-OpenTime);
			DoorTR[2]->AddPosition_Y(-OpenTime);

			DoorTR[1]->AddPosition_Y(OpenTime);
			DoorTR[3]->AddPosition_Y(OpenTime);

			OpenTime = 0.0f;
			IS_OPEN = false;
		}
	}
	else if (IS_CLOSE == true)
	{
		OpenTime = GetDeltaTime() * 4.0f;
		if (DoorTR[0] != nullptr && DoorTR[2] != nullptr)
		{
			DoorTR[0]->AddPosition_Y(-OpenTime);
			DoorTR[2]->AddPosition_Y(-OpenTime);
		}

		if (DoorTR[1] != nullptr && DoorTR[3] != nullptr)
		{
			DoorTR[1]->AddPosition_Y(OpenTime);
			DoorTR[3]->AddPosition_Y(OpenTime);
		}

		if (DoorTR[0]->GetPosition().y <= 0.0f)
		{
			DoorTR[0]->SetPosition_Y(0.0f);
			DoorTR[2]->SetPosition_Y(0.0f);
			DoorTR[1]->SetPosition_Y(0.0f);
			DoorTR[3]->SetPosition_Y(0.0f);
			OpenTime = 0.0f;
			IS_CLOSE = false;
		}
	}
}

void GateDoor::Open(int _1, int _2)
{
	IS_OPEN = true;
	DoorTR[0] = Door[_1]->GetChildMesh(0)->GetTransform();
	DoorTR[1] = Door[_1]->GetChildMesh(1)->GetTransform();
	DoorTR[2] = Door[_2]->GetChildMesh(0)->GetTransform();
	DoorTR[3] = Door[_2]->GetChildMesh(1)->GetTransform();
}

void GateDoor::Close(int _1, int _2)
{
	IS_CLOSE = true;
	DoorTR[0] = Door[_1]->GetChildMesh(0)->GetTransform();
	DoorTR[1] = Door[_1]->GetChildMesh(1)->GetTransform();

	DoorTR[2] = Door[_2]->GetChildMesh(0)->GetTransform();
	DoorTR[3] = Door[_2]->GetChildMesh(1)->GetTransform();
}
