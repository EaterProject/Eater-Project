#include "ManaStone.h"
#include "GameObject.h"
#include "MeshFilter.h"
#include "Transform.h"
#include "EaterEngineAPI.h"
#include "ObjectFactory.h"
#include "MonsterA.h"

std::vector<Vector3> ManaStone::MonsterMovePoint;
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

	//한번만 5방향의 꼭지점을 구한다
	if ((int)MonsterMovePoint.size() == 0){CreateMonsterRangePoint(5);}

	Vector3 Poistion = MonsterMovePoint[0] * 10;
	MonsterA* Monster = ObjectFactory::CreateMonsterA(0, 0, 0);
	Monster->Mana = this;
	MonsterList.push_back(Monster);
}

void ManaStone::SetUp()
{
	mMeshFilter->SetModelName("mana");
}

void ManaStone::Update()
{
	Debug();
}

Vector3 ManaStone::GetPoint(int CreateRangeIndex, int MovePointIndex)
{
	//나의 생성위치에서의 5방향의 꼭지점 그방향으로의 범위값을 곱함
	Vector3 MonsterPoint = GetMonsterPoint(mTransform->Position, CreateRangeIndex, Range * 0.5f);
	return GetMonsterPoint(MonsterPoint, MovePointIndex, Range * 0.25f);
}

void ManaStone::Debug()
{
	Vector3 Point = mTransform->Position;
	Point.y += 1;
	DebugDrawCircle(10, Point, Vector3(0, 0, 0), Vector3(1, 0, 0));

	//디버깅을 그린다
	int Size = (int)MonsterMovePoint.size();
	for (int i = 0; i < Size; i++)
	{
		float DebugHeight = 1;		//디버깅 높이

		//나의 생성위치에서의 5방향의 꼭지점 그방향으로의 범위값을 곱함
		Vector3 MonsterPoint = GetMonsterPoint(mTransform->Position, i, Range *0.5);
		DebugDrawLine(Point, MonsterPoint, Vector3(0, 0, 1));
		DebugDrawCircle(Range * 0.25f, MonsterPoint, Vector3(0, 0, 0), Vector3(0, 0, 1));


		////5방향에 대한 원
		for (int j = 0; j < 5; j++)
		{
			Vector3 Point = GetMonsterPoint(MonsterPoint, j, Range * 0.25f);
			DebugDrawCircle(0.25f, Point, Vector3(0, 0, 0), Vector3(0, 1, 0));
		}
	}
}

void ManaStone::CreateMonsterRangePoint(int MonsterCount)
{
	//1마리 일때
	if (MonsterCount == 1)
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

		Vector3 EndPoint = Vector3(0, 0, 0);
		EndPoint.x += X;
		EndPoint.y += 1;
		EndPoint.z += y;
		MonsterMovePoint.push_back(EndPoint);
	}
}

Vector3 ManaStone::GetMonsterPoint(const Vector3& MyPosition, int MonsterPointIndex, float Range)
{
	if (MonsterPointIndex >= 5) { return Vector3(0, 0, 0); }

	Vector3 Point = MyPosition + (MonsterMovePoint[MonsterPointIndex] * Range);
	Point.y = 1;
	return Point;
}
