#include "Store.h"
#include "EaterEngineAPI.h"
#include "MessageManager.h"


#include "GameObject.h"
#include "MeshFilter.h"
#include "Transform.h"
#include "Collider.h"
#include "RectTransform.h"
#include "Image.h"

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

	GameObject* object = Instance_Image();
	RectTransform* trans = object->GetComponent<RectTransform>();
	trans->SetPivot(PIVOT_TYPE::PIVOT_OBJECT);
	trans->SetPositionObject(mTransform, Vector3(-1.0f, 1.7f, 0.0f));
	mStoreActive = object->GetComponent<Image>();
	mStoreActive->SetTexture("Store_UI");
	mStoreActive->SetActive(false);
}

void Store::SetUp()
{
	mCollider->SetSphereCollider(3);
	mCollider->SetCenter(0, 1, 0);
	mCollider->SetTrigger(true);
	mMeshFilter->SetModelName("Dome_VendingMachine");
	mTransform->SetPosition(16.5f, 0, 0);
}

void Store::Update()
{
	if (IsRange)
	{
		if (GetKeyDown('F'))
		{
			mStoreActive->SetActive(false);

			MessageManager::GetGM()->SEND_Message(TARGET_GLOBAL, MESSAGE_GLOBAL_STORE);
		}
	}
}

void Store::Debug()
{
	DebugDrawCircle(3, mTransform->GetPosition(), Vector3(0, 0, 0), Vector3(1, 0, 0));
}

void Store::StoreActive(int Type)
{
	switch (Type)
	{
	case MESSAGE_STORE_EXIT:
		mStoreActive->SetActive(true);
		break;
	case MESSAGE_STORE_PAUSE:
		mStoreActive->SetActive(false);
		break;
	default:
		break;
	}
}

void Store::OnTriggerEnter(GameObject* Obj)
{
	if (Obj->GetTag() == 0)
	{
		//범위 안에 들어왔다면  Player UI를 끄고 상점 UI를 켠다
		IsRange = true;
		mStoreActive->SetActive(true);
	}
}

void Store::OnTriggerExit(GameObject* Obj)
{
	if (Obj->GetTag() == 0)
	{
		IsRange = false;
		mStoreActive->SetActive(false);
	}
}
