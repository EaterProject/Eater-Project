#include "ManaStone.h"
#include "GameObject.h"
#include "MeshFilter.h"
#include "Transform.h"
#include "MainHeader.h"
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
	mMeshFilter->SetModelName("box");
}

void ManaStone::Update()
{
	

}

void ManaStone::ManaStoneUpdate(Vector3 Pos)
{
	//�ִϸ��̼� �����͵� ���� ����
	mTransform->Position = Pos;
}

void ManaStone::ReSet()
{
}
