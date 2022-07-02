#include "BossStateManager.h"
#include "GameObject.h"

//컨포넌트
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

	//보스 오브젝트의 컨포넌트를 가져옴
	mAnime		= BossObject->GetComponent<AnimationController>();
	mCollider	= BossObject->GetComponent<Collider>();
	mRigidbody  = BossObject->GetComponent<Rigidbody>();
	mTransform  = BossObject->GetTransform();
}

void BossStateManager::Idle()
{
		 





}
