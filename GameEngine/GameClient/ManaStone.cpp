#include "ManaStone.h"
#include "GameObject.h"
#include "MeshFilter.h"
#include "Transform.h"
#include "EaterEngineAPI.h"
#include "ObjectFactory.h"
#include <time.h>
#include "MonsterA.h"
#include "MonsterB.h"

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

	//�ѹ��� 5������ �������� ���Ѵ�
	if ((int)MonsterMovePoint.size() == 0){CreateMonsterRangePoint(5);}

	srand((unsigned int)time(NULL));

	CreateMonster(5, 0);
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
	//���� ������ġ������ 5������ ������ �׹��������� �������� ����
	Vector3 MonsterPoint = GetMonsterPoint(mTransform->Position, CreateRangeIndex, Range * 0.5f);
	return GetMonsterPoint(MonsterPoint, MovePointIndex, Range * 0.25f);
}

void ManaStone::CreateMonster(int MonsterACount, int MonsterBCount)
{
	int index = 0;
	bool CreatePointCheck[5] = { false, };
	for (int i = 0; i < 5; i++)
	{
		if (MonsterACount != 0)
		{
			do 
			{
				index = rand() % 5;
				
			}
			while (CreatePointCheck[index]);

			CreateMonsterA(index);
			CreatePointCheck[index] = true;
			MonsterACount--;
		}
		else if (MonsterBCount != 0)
		{
			do
			{
				index = rand() % 5;

			} while (CreatePointCheck[index]);

			CreateMonsterB(index);
			CreatePointCheck[index] = true;
			MonsterBCount--;
		}
	}
}

void ManaStone::Debug()
{
	Vector3 Point = mTransform->Position;
	Point.y = 0.25f;
	DebugDrawCircle(10, Point, Vector3(0, 0, 0), Vector3(1, 0, 0));

	//������� �׸���
	int Size = (int)MonsterMovePoint.size();
	for (int i = 0; i < Size; i++)
	{
		float DebugHeight = 0;		//����� ����

		//���� ������ġ������ 5������ ������ �׹��������� �������� ����
		Vector3 MonsterPoint = GetMonsterPoint(mTransform->Position, i, Range *0.5);
		MonsterPoint.y = 0.25f;
		DebugDrawLine(Point, MonsterPoint, Vector3(0, 0, 1));
		DebugDrawCircle(Range * 0.25f, MonsterPoint, Vector3(0, 0, 0), Vector3(0, 0, 1));


		////5���⿡ ���� ��
		for (int j = 0; j < 5; j++)
		{
			Vector3 Point = GetMonsterPoint(MonsterPoint, j, Range * 0.25f);
			Point.y = 0.25f;
			DebugDrawCircle(0.25f, Point, Vector3(0, 0, 0), Vector3(0, 1, 0));
		}
	}
}

void ManaStone::CreateMonsterRangePoint(int MonsterCount)
{
	///���� ���� ��ġ�� ���Ѵ�
	//1���� �϶�
	if (MonsterCount == 1) { return; }

	//�־��� �������� ������ ���� ����ŭ ���и�
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

void ManaStone::CreateMonsterA(int index)
{
	///���� A�� ����
	Vector3 Poistion = MonsterMovePoint[index] * 10;
	MonsterA* Monster = ObjectFactory::CreateMonsterA(Poistion.x, 0, Poistion.z);
	Monster->Create(this, index);
	MonsterA_List.push_back(Monster);
}

void ManaStone::CreateMonsterB(int index)
{
	///���� B�� ����
	Vector3 Poistion = MonsterMovePoint[index] * 10;
	MonsterB* Monster = ObjectFactory::CreateMonsterB(Poistion.x, 0, Poistion.z);
	Monster->Create(this, index);
	MonsterB_List.push_back(Monster);
}

Vector3 ManaStone::GetMonsterPoint(const Vector3& MyPosition, int MonsterPointIndex, float Range)
{
	if (MonsterPointIndex >= 5) { return Vector3(0, 0, 0); }
	Vector3 Point = MyPosition + (MonsterMovePoint[MonsterPointIndex] * Range);
	Point.y = 1;
	return Point;
}
