#include "BossStateManager.h"
#include "GameObject.h"

//������Ʈ
#include "Rigidbody.h"
#include "Collider.h"
#include "AnimationController.h"


BossStateManager::BossStateManager()
{



}

BossStateManager::~BossStateManager()
{



}

void BossStateManager::ObjectSetting(GameObject* BossObject)
{
	if (BossObject == nullptr) { return; }

	//���� ������Ʈ�� ������Ʈ�� ������
	mAnime		= BossObject->GetComponent<AnimationController>();
	mCollider	= BossObject->GetComponent<Collider>();
	mRigidbody  = BossObject->GetComponent<Rigidbody>();
	mTransform  = BossObject->GetTransform();
}

void BossStateManager::Idle()
{
		 





}
