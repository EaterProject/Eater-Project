#include "Monster.h"
#include "MeshFilter.h"
#include "Transform.h"
#include "GameObject.h"
#include "AnimationController.h"
#include "MainHeader.h"

Monster::Monster()
{
	mMeshFilter = nullptr;
	mTransform	= nullptr;
	mAnimation	= nullptr;
}

Monster::~Monster()
{
	mMeshFilter = nullptr;
	mTransform	= nullptr;
	mAnimation	= nullptr;
}

void Monster::Awake()
{
	mMeshFilter = gameobject->GetComponent<MeshFilter>();
	mTransform	= gameobject->GetComponent<Transform>();
	mAnimation	= gameobject->GetComponent<AnimationController>();
}
void Monster::SetUp()
{
	mMeshFilter->SetModelName("MonsterA");
	mMeshFilter->SetAnimationName("MonsterA");

	//mTransform->Rotation = { -90,0,0 };
	//mTransform->Position = { -43,65,-6 };

	//mTransform->Rotation = { 90,0,0 };
	mTransform->Scale = {2,2,2};
	mTransform->Position = { -5,0,0 };
	mTransform->Rotation = { 90,180,0 };

	mAnimation->Choice("idle");
}

void Monster::Update()
{
	mAnimation->Play(1, true);
	//µð¹ö±ë¿ë
}

void Monster::MonsterUpdate(Vector3 Pos)
{
	mTransform->Position = Pos;
}
