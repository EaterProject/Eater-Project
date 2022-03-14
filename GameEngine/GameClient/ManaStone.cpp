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
	mMeshFilter->SetMeshName("box");
}

void ManaStone::Update()
{
	DebugDrawLine(mTransform->Position, mTransform->Position + Vector3(1, 0, 0), Vector4(1, 0, 0, 1));
	DebugDrawLine(mTransform->Position, mTransform->Position + Vector3(0, 1, 0), Vector4(0, 1, 0, 1));
	DebugDrawLine(mTransform->Position, mTransform->Position + Vector3(0, 0, 1), Vector4(0, 0, 1, 1));

}

void ManaStone::ManaStoneUpdate(Vector3 Pos)
{
	//애니메이션 데이터도 들어올 예정
	mTransform->Position = Pos;
}
