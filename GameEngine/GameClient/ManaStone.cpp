#include "ManaStone.h"
#include "GameObject.h"
#include "MeshFilter.h"
#include "Transform.h"
#include "EaterEngineAPI.h"
ManaStone::ManaStone()
{
	mMeshFilter = nullptr;
	mTransform	= nullptr;
	mAnimation	= nullptr;
}

ManaStone::~ManaStone()
{
	mMeshFilter = nullptr;
	mTransform	= nullptr;
	mAnimation	= nullptr;
}

void ManaStone::Awake()
{
	mMeshFilter = gameobject->GetComponent<MeshFilter>();
	mTransform	= gameobject->GetComponent<Transform>();
}

void ManaStone::SetUp()
{
	mMeshFilter->SetModelName("mana");
}

void ManaStone::Update()
{
	DebugDrawCircle(10, Vector3(0, 5, 0), Vector3(0, 0, 0), Vector3(1,0,0));
}

void ManaStone::ManaStoneUpdate(Vector3 Pos)
{
	//�ִϸ��̼� �����͵� ���� ����
	mTransform->Position = Pos;
}

void ManaStone::ReSet()
{
}
