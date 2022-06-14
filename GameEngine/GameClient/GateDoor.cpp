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




}

void GateDoor::Open(int _1, int _2)
{
	UP_Front = Door[_1]->GetChildMesh(0);
	Down_Front = Door[_1]->GetChildMesh(1);
	UP_Back = Door[_2]->GetChildMesh(0);
	Down_Back = Door[_2]->GetChildMesh(1);

	
}

void GateDoor::Close(int _1, int _2)
{
	UP_Front = Door[_1]->GetChildMesh(0);
	Down_Front = Door[_1]->GetChildMesh(1);
	UP_Back = Door[_2]->GetChildMesh(0);
	Down_Back = Door[_2]->GetChildMesh(1);
}
