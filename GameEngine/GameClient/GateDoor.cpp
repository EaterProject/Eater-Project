#include "GateDoor.h"
#include "MessageManager.h"
#include "EaterEngineAPI.h"
#include "GameObject.h"
#include "Transform.h"
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
}

void GateDoor::Start()
{
	Door[0]->GetTransform()->Rotation = { 0,324,0 };
	Door[1]->GetTransform()->Rotation = { 0,324,0 };

	Door[2]->GetTransform()->Rotation = { 0,36,0 };
	Door[3]->GetTransform()->Rotation = { 0,36,0 };

	Door[4]->GetTransform()->Rotation = { 0,144,0 };
	Door[5]->GetTransform()->Rotation = { 0,144,0 };
}

void GateDoor::SetOpen(int Number)
{
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
	}
}

void GateDoor::SetClose(int Number)
{
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
	}
}

void GateDoor::Update()
{
	if (IS_OPEN == true)
	{
		OpenTime = GetDeltaTime()*4.0f;
		if (DoorTR[0] != nullptr && DoorTR[2] != nullptr)
		{
			DoorTR[0]->Position.y += OpenTime;
			DoorTR[2]->Position.y += OpenTime;
		}

		if (DoorTR[1] != nullptr && DoorTR[3] != nullptr)
		{
			DoorTR[1]->Position.y += -OpenTime;
			DoorTR[3]->Position.y += -OpenTime;
		}

		if (DoorTR[0]->Position.y >= 4.0f)
		{
			DoorTR[0]->Position.y += -OpenTime;
			DoorTR[2]->Position.y += -OpenTime;

			DoorTR[1]->Position.y += OpenTime;
			DoorTR[3]->Position.y += OpenTime;

			OpenTime = 0.0f;
			IS_OPEN = false;
		}
	}
	else if (IS_CLOSE == true)
	{
		OpenTime = GetDeltaTime();
		if (DoorTR[0] != nullptr && DoorTR[2] != nullptr)
		{
			DoorTR[0]->Position.y += OpenTime * -1;
			DoorTR[2]->Position.y += OpenTime * -1;
		}

		if (DoorTR[1] != nullptr && DoorTR[3] != nullptr)
		{
			DoorTR[1]->Position.y += OpenTime;
			DoorTR[3]->Position.y += OpenTime;
		}

		if (DoorTR[0]->Position.y <= 0.0f)
		{
			DoorTR[0]->Position.y = 0.0f;
			DoorTR[2]->Position.y = 0.0f;
			DoorTR[1]->Position.y = 0.0f;
			DoorTR[3]->Position.y = 0.0f;
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
