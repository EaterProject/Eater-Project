#include "Store.h"
#include "EaterEngineAPI.h"
#include "MessageManager.h"


#include "GameObject.h"
#include "MeshFilter.h"
#include "Transform.h"
#include "Collider.h"
Store::Store()
{

}

Store::~Store()
{

}

void Store::Awake()
{
	mCollider	= gameobject->GetComponent<Collider>();
	mMeshFilter = gameobject->GetComponent<MeshFilter>();
	mTransform	= gameobject->GetTransform();

	PlayerTag = FindTagNumber("Player");
}

void Store::SetUp()
{
	mCollider->SetSphereCollider(3);
	mCollider->SetCenter(0, 1, 0);
	mCollider->SetTrigger(true);
	mMeshFilter->SetModelName("Dome_VendingMachine");
	mTransform->SetPosition(16.5f, 0, 0);
}

void Store::Debug()
{
	DebugDrawCircle(3, mTransform->GetPosition(), Vector3(0, 0, 0), Vector3(1, 0, 0));
}

void Store::OnTriggerEnter(GameObject* Obj)
{
	if (Obj->GetTag() == 0)
	{
		//범위 안에 들어왔다면  Player UI를 끄고 상점 UI를 켠다
		int num = 0;
		bool ON = true;
		MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_STORE_ACTIVE, &ON);
		ON = false;
		MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_PLAYER_ACTIVE, &ON);
	}
}

void Store::OnTriggerExit(GameObject* Obj)
{
	if (Obj->GetTag() == 0)
	{
		//범위 안에 들어왔다면  Player UI를 켜도 상점 UI를 끈다
		int num = 0;
		bool ON = false;
		MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_STORE_ACTIVE, &ON);
		ON = true;
		MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_PLAYER_ACTIVE, &ON);
	}
}
