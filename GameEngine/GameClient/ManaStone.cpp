#include "ManaStone.h"
#include "GameObject.h"
#include "MeshFilter.h"
#include "Transform.h"
#include "EaterEngineAPI.h"

#define LERP(prev, next, time) ((prev * (1.0f - time)) + (next * time))
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
	CreateMonsterRangePoint(5);
	
}

void ManaStone::Update()
{
	Vector3 Point = mTransform->Position;
	Point.y += 1;
	DebugDrawCircle(10, Point, Vector3(0, 0, 0), Vector3(1, 0, 0));

	int Size = (int)TrianglePoint.size();
	for (int i = 0; i < Size; i++)
	{
		Vector3 MonsterPoint = TrianglePoint[i] * 5;
		MonsterPoint.y = 1;
		DebugDrawLine(Point, MonsterPoint, Vector3(0, 0, 1));
	}

	Size = (int)TriangleCenterPoint.size();
	for (int i = 0; i < Size; i++)
	{
		Vector3 MonsterPoint = TrianglePoint[i] * 5;
		MonsterPoint.y = 1;
		DebugDrawCircle(TrianglePointDir, MonsterPoint, Vector3(0, 0, 0), Vector3(0, 0, 1));
	}
}

void ManaStone::ManaStoneUpdate(Vector3 Pos)
{
	//애니메이션 데이터도 들어올 예정
	mTransform->Position = Pos;
}

void ManaStone::ReSet()
{
	

}

void ManaStone::CreateMonsterRangePoint(int MonsterCount)
{
	
	//1마리 일때
	if (MonsterCount == 1)
	{

	}
	//2마리 일때
	if (MonsterCount == 2)
	{

	}
	//5마리 이상
	if (MonsterCount > 5)
	{

	}


	//주어진 범위에서 몬스터의 생성 수만큼 원분리
	float Angle = 360 / MonsterCount;
	float NowAngle = 90;
	for (int i = 0; i < MonsterCount; i++)
	{
		NowAngle += Angle;
		float X = cos(NowAngle * (3.141592f / 180));
		float y = sin(NowAngle * (3.141592f / 180));

		Vector3 EndPoint = mTransform->Position;
		EndPoint.x += X;
		EndPoint.y += 1;
		EndPoint.z += y;

		TrianglePoint.push_back(EndPoint);
	}

	//TrianglePointDir = sqrt(pow((TrianglePoint[0].x *10) - (TrianglePoint[1].x*10), 2) + pow((TrianglePoint[0].z *10) - (TrianglePoint[1].z *10), 2)) * 0.4f;
	//
	//
	////분리된 꼭지점 들의 중심점을 구한다
	//for (int i = 0;i < MonsterCount; i++)
	//{
	//	if(i + 1 > MonsterCount-1) 
	//	{
	//		Vector3 Start	= TrianglePoint[i] * 10;
	//		Vector3 End		= TrianglePoint[0] * 10;
	//		Start.y = 1;
	//		End.y = 1;
	//		TriangleCenterPoint.push_back(LERP(Start, End, 0.5f));
	//	}
	//	else
	//	{
	//		Vector3 Start = TrianglePoint[i] * 10;
	//		Vector3 End = TrianglePoint[i+1] * 10;
	//		Start.y = 1;
	//		End.y = 1;
	//		TriangleCenterPoint.push_back(LERP(Start, End, 0.5f));
	//	}
	//}
}
