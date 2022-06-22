#include "BossFriend.h"
#include "MeshFilter.h"
#include "Transform.h"
#include "GameObject.h"
#include "AnimationController.h"
#include "Player.h"

BossFriend::BossFriend()
{
}

BossFriend::~BossFriend()
{


}

void BossFriend::Awake()
{
	mTransform	= gameobject->GetTransform();
	mMeshFilter = gameobject->GetComponent<MeshFilter>();
	mAnimation	= gameobject->GetComponent<AnimationController>();
	mTransform->SetScale(1.5f, 1.5f, 1.5f);
}

void BossFriend::SetUp()
{
	mMeshFilter->SetModelName("BossB+");
	mMeshFilter->SetAnimationName("BossB+");
	gameobject->SetActive(false);
}

void BossFriend::Update()
{
	mTransform->Slow_Y_Rotation(Player::GetPlayerTransform()->GetPosition(), 150);
	//mAnimation->Choice("idle");
	//mAnimation->Play();
}

void BossFriend::SetPosition(Vector3 Pos)
{
	gameobject->SetActive(true);
	mTransform->SetPosition(Pos);
}

void BossFriend::SetPosition(float x, float y, float z)
{
	gameobject->SetActive(true);
	mTransform->SetPosition(x, y, z);
}
