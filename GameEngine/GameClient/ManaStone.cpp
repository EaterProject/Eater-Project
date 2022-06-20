#include "ManaStone.h"
#include "GameObject.h"
#include "MeshFilter.h"
#include "Transform.h"
#include "EaterEngineAPI.h"
#include "ObjectFactory.h"
#include <time.h>
#include "MonsterA.h"
#include "MonsterB.h"
#include "MessageManager.h"

std::vector<Vector3> ManaStone::MonsterMovePointDefault;
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
	if ((int)MonsterMovePointDefault.size() == 0)
	{
		CreateMonsterRangePoint(5);
	}

	srand((unsigned int)time(NULL));

	CreateMonster(MonsterACount, MonsterBCount);
}

void ManaStone::SetUp()
{
	mMeshFilter->SetModelName("Mana");
}

void ManaStone::Update()
{
	Debug();
}

void ManaStone::SetMonsterCount(int MonsterA, int MonsterB)
{
	MonsterACount = MonsterA;
	MonsterBCount = MonsterB;
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
	Point.y = 2.0f;
	DebugDrawCircle(10, Point, Vector3(0, 0, 0), Vector3(1, 0, 0));

	//������� �׸���
	int Size = (int)MonsterMovePointDefault.size();
	for (int i = 0; i < Size; i++)
	{
		float DebugHeight = 0;		//����� ����

		//���� ������ġ������ 5������ ������ �׹��������� �������� ����
		Vector3 MonsterPoint = GetMonsterPoint(mTransform->Position, i, Range *0.5);
		MonsterPoint.y = 2.0f;
		DebugDrawLine(Point, MonsterPoint, Vector3(0, 0, 1));
		DebugDrawCircle(Range * 0.25f, MonsterPoint, Vector3(0, 0, 0), Vector3(0, 0, 1));


		////5���⿡ ���� ��
		for (int j = 0; j < 5; j++)
		{
			Vector3 Point = GetMonsterPoint(MonsterPoint, j, Range * 0.25f);
			Point.y = 2.0f;
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
		MonsterMovePointDefault.push_back(EndPoint);
	}
}

void ManaStone::CreateMonsterA(int index)
{
	///���� A�� ����
	GameObject* Object = MessageManager::GetGM()->CREATE_MESSAGE(TARGET_MONSTER_A);
	MonsterA* Monster = Object->GetComponent<MonsterA>();
	//���Ϳ��� �߰��� ����Ʈ 5���� �ǳ��ش�
	for (int i = 0; i < 5; i++)
	{
		Vector3 Point = GetPoint(index, i);
		Point.y = mTransform->Position.y+1;
		Monster->SetSearchPoint(i, Point);
	}
	MonsterA_List.push_back(Monster);
}

void ManaStone::CreateMonsterB(int index)
{
	///���� B�� ����
	GameObject* Object	= MessageManager::GetGM()->CREATE_MESSAGE(TARGET_MONSTER_B);
	MonsterB* Monster	= Object->GetComponent<MonsterB>();
	for (int i = 0; i < 5; i++)
	{
		Vector3 Point = GetPoint(index, i);
		Point.y = mTransform->Position.y;
		Monster->SetSearchPoint(i, Point);
	}
	MonsterB_List.push_back(Monster);
}

Vector3 ManaStone::GetMonsterPoint(const Vector3& MyPosition, int MonsterPointIndex, float Range)
{
	if (MonsterPointIndex >= 5) { return Vector3(0, 0, 0); }
	Vector3 Point = MyPosition + (MonsterMovePointDefault[MonsterPointIndex] * Range);
	Point.y = 1;
	return Point;
}
