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
#include "Player.h"
#include "Collider.h"
#include "Rigidbody.h"
#include "MiniMapSystem.h"

#include "ParticleController.h"
#include "ParticleFactory.h"

std::vector<Vector3> ManaStone::MonsterMovePointDefault;
int ManaStone::MaxManaCount = 5;

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
	mCollider = gameobject->GetComponent<Collider>();
	mRigidbody = gameobject->GetComponent<Rigidbody>();

	//한번만 5방향의 꼭지점을 구한다
	if ((int)MonsterMovePointDefault.size() == 0)
	{
		CreateMonsterRangePoint(5);
	}

	srand((unsigned int)time(NULL));

	CreateMonster(MonsterACount, MonsterBCount);
	mAttackParticle		= ParticleFactory::Get()->CreateParticleController(PARTICLE_TYPE::ManaTreeSmoke);
	mRangeParticle	= ParticleFactory::Get()->CreateParticleController(PARTICLE_TYPE::ManaSmoke);
}

void ManaStone::SetUp()
{
	mMeshFilter->SetModelName("Mana");
	mCollider->SetCenter(0, 0.5f, 0);
	mCollider->SetBoxCollider(0.1f,0.1f, 0.1f);
	mRigidbody->SetFreezeRotation(true, true, true);
	mRigidbody->SetFreezePosition(true, true, true);
	mTransform->SetScale(1.7f, 1.7f, 1.7f);
}

void ManaStone::Start()
{
	mSetting.Setting(this->gameobject);
	mAttackParticle->Play();


	Vector3 Pos = mTransform->GetPosition();
	Pos.y += 1.5f;
	mRangeParticle->SetPosition(Pos);
	mRangeParticle->Play();
}

void ManaStone::Update()
{
	mSetting.LimLightUpdate(2.8f);
}

void ManaStone::SetMonsterCount(int MonsterA, int MonsterB)
{
	MonsterACount = MonsterA;
	MonsterBCount = MonsterB;
}

Vector3 ManaStone::GetPoint(int CreateRangeIndex, int MovePointIndex)
{
	//나의 생성위치에서의 5방향의 꼭지점 그방향으로의 범위값을 곱함
	Vector3 MonsterPoint = GetMonsterPoint(mTransform->GetPosition(), CreateRangeIndex, Range * 0.5f);
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

void ManaStone::Delete()
{
	//int A_Size = MonsterA_List.size();
	//int B_Size = MonsterA_List.size();
	//for (int i = 0; i < A_Size; i++)
	//{
	//	Destroy(MonsterA_List[i]->gameobject);
	//}
	//
	//for (int i = 0; i < B_Size; i++)
	//{
	//	Destroy(MonsterB_List[i]->gameobject);
	//}

	Destroy(this->gameobject);
	MiniMapSystem::Get()->DeleteIcon(this->gameobject);
	mAttackParticle->Stop();
	mRangeParticle->Stop();

	mAttackParticle->gameobject->SetActive(false);
	mRangeParticle->gameobject->SetActive(false);
}

void ManaStone::Debug()
{
	Vector3 Point = mTransform->GetPosition();
	Point.y = 2.0f;
	DebugDrawCircle(10, Point, Vector3(0, 0, 0), Vector3(1, 0, 0));

	//디버깅을 그린다
	int Size = (int)MonsterMovePointDefault.size();
	for (int i = 0; i < Size; i++)
	{
		float DebugHeight = 0;		//디버깅 높이

		//나의 생성위치에서의 5방향의 꼭지점 그방향으로의 범위값을 곱함
		Vector3 MonsterPoint = GetMonsterPoint(mTransform->GetPosition(), i, Range *0.5);
		MonsterPoint.y = 2.0f;
		DebugDrawLine(Point, MonsterPoint, Vector3(0, 0, 1));
		DebugDrawCircle(Range * 0.25f, MonsterPoint, Vector3(0, 0, 0), Vector3(0, 0, 1));

		////5방향에 대한 원
		for (int j = 0; j < 5; j++)
		{
			Vector3 Point = GetMonsterPoint(MonsterPoint, j, Range * 0.25f);
			Point.y = 2.0f;
			DebugDrawCircle(0.25f, Point, Vector3(0, 0, 0), Vector3(0, 1, 0));
		}
	}
}

void ManaStone::OnTriggerStay(GameObject* Obj)
{
	//플레이어 충돌체와 충돌했을때
	if (HitStart == false && DeadStart == false)
	{
		//플레이어가 공격 상태일때
		if (Player::GetAttackState() == true)
		{
			Sound_Play_SFX("ManaLeaf_Hit");
			MessageManager::GetGM()->SEND_Message(TARGET_PLAYER, MESSAGE_PLAYER_ATTACK_OK);
			mSetting.SetLimlightSetting(MeshFilterSetting::COLOR_TYPE::RED, 1.50f, 2.0f);
			mSetting.SetLimlightSettingMax(MeshFilterSetting::COLOR_TYPE::RED, 0.0f, 0.0f);

			HP -= Player::GetPlayerPower();
			if (HP <= 0)
			{
				mSetting.SetDissolveOption(DISSOLVE_FADEOUT);
				mSetting.SetDissolveTexture("Dissolve_1");
				mSetting.SetDissolveColor(255.0f, 0, 0);
				mSetting.SetDissolveColorFactor(10.0f);
				mSetting.SetDissolvePlayTime(8.0f);
				mSetting.SetDissolveWidth(0.1f);
				mSetting.SetDissolveInnerFactor(100.0f);
				mSetting.SetDissolveOuterFactor(25.0f);
				mSetting.PlayDissolve();
				DeadStart = true;
				MiniMapSystem::Get()->DeleteIcon(this->gameobject);

				// 보스 등장 전 마나 개수..
				if (Boss_Start == false)
				{
					MaxManaCount--;

					if (MaxManaCount <= 0)
					{
						Boss_Start = true;
						MaxManaCount = 0;
						MessageManager::GetGM()->SEND_Message(TARGET_GLOBAL, MESSAGE_GLOBAL_BOSS_START);
					}
				}

				mAttackParticle->Stop();
				mRangeParticle->Stop();
				// 코어 마나 획득
				MessageManager::GetGM()->SEND_Message(TARGET_PLAYER, MESSAGE_PLAYER_GET_COREMANA, &CoreManaCount);
			}

			HitStart = true;
		}
	}
	else
	{
		if (Player::GetAttackState() == false)
		{
			//플레이어 상태가 공격상태가 아닐떄
			if (Player::GetAttackState() == false)
			{
				HitStart = false;
			}
		}
	}
}

void ManaStone::CreateMonsterRangePoint(int MonsterCount)
{
	///몬스터 생성 위치를 구한다
	//1마리 일때
	if (MonsterCount == 1) { return; }

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
		MonsterMovePointDefault.push_back(EndPoint);
	}
}

void ManaStone::CreateMonsterA(int index)
{
	///몬스터 A를 생성
	GameObject* Object = MessageManager::GetGM()->CREATE_MESSAGE(TARGET_MONSTER_A);
	MonsterA* Monster = Object->GetComponent<MonsterA>();
	//몬스터에게 추격할 포인트 5곳을 건내준다
	for (int i = 0; i < 5; i++)
	{
		Vector3 Point = GetPoint(index, i);
		Point.y = mTransform->GetPosition().y+2;
		Monster->SetSearchPoint(i, Point);
	}
	MonsterA_List.push_back(Monster);
}

void ManaStone::CreateMonsterB(int index)
{
	///몬스터 B를 생성
	GameObject* Object	= MessageManager::GetGM()->CREATE_MESSAGE(TARGET_MONSTER_B);
	MonsterB* Monster	= Object->GetComponent<MonsterB>();
	for (int i = 0; i < 5; i++)
	{
		Vector3 Point = GetPoint(index, i);
		Point.y = mTransform->GetPosition().y+3;
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
