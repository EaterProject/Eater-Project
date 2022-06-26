#include "PlayerCollider.h"
#include "EaterEngineAPI.h"
#include "MessageManager.h"

//Component
#include "GameObject.h"
#include "Collider.h"
#include "Rigidbody.h"
#include "Player.h"
#include "Transform.h"
PlayerCollider::PlayerCollider()
{

}

PlayerCollider::~PlayerCollider()
{

}

void PlayerCollider::Awake()
{
	mRigidbody	= gameobject->GetComponent<Rigidbody>();
	mCollider	= gameobject->GetComponent<Collider>();
	mTransform	= gameobject->GetTransform();
	mPlayerTR	= Player::GetPlayerTransform();
}

void PlayerCollider::SetUp()
{
	mCollider->SetSphereCollider(0.25f);
	mCollider->SetCenter(0,1.0f,0);
	mRigidbody->SetGravity(false);

	Gate_01_IN_Tag = FindTagNumber("Gate_01_IN");
	Gate_01_OUT_Tag = FindTagNumber("Gate_01_OUT");
}

void PlayerCollider::Update()
{
	mTransform->SetPosition(mPlayerTR->GetPosition());
}

void PlayerCollider::Debug()
{
	
}

void PlayerCollider::OnTriggerEnter(GameObject* Obj)
{
	
}

void PlayerCollider::OnTriggerExit(GameObject* Obj)
{
	
}

void PlayerCollider::OnTriggerStay(GameObject* Obj)
{

}
