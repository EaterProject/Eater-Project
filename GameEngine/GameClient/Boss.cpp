#include "Boss.h"
#include "MessageManager.h"
#include <functional>
#include "EaterEngineAPI.h"
#include "EngineData.h"

//component
#include "GameObject.h"
#include "Transform.h"
#include "Player.h"
#include "Collider.h"
#include "Rigidbody.h"
#include "MeshFilter.h"
#include "AnimationController.h"

Boss::Boss()
{
}

Boss::~Boss()
{
}

void Boss::Awake()
{
	MonsterComponent::Awake();
}

void Boss::SetUp()
{
	ModelName		= "BossB+";
	AnimationName	= "BossB+";

	
	mColider->SetSphereCollider(0.5f);
	mColider->SetCenter(0, 0.5f, 0);
	mColider->SetMaterial_Restitution(0);
	mRigidbody->SetFreezeRotation(true, true, true);

	mMeshFilter->SetModelName(ModelName);
	mMeshFilter->SetAnimationName(AnimationName);

	mTransform->Position = { 16.5f,0.5f,53.5f };

	HP = 100000;
}

void Boss::Update()
{
	UpdateColor();
	mAnimation->Choice("idle");
	mAnimation->Play();
}

void Boss::Debug()
{



}

void Boss::OnTriggerStay(GameObject* Obj)
{
	if (HitStart == false)
	{
		//�÷��̾ ���� �����϶�
		if (Player::GetAttackState() == true)
		{
			MessageManager::GetGM()->SEND_Message(TARGET_PLAYER, MESSAGE_PLAYER_ATTACK_OK);
			//���� �ٲٴ� �Լ������͸� �ְ� ���º�ȭ
			HitFunction = std::bind(&Boss::SetLimLightColor, this);
			SetMonsterState(MONSTER_STATE::HIT);
			Hit();
			HP -= 20;
			HitStart = true;
			//���� ���
			Sound_Play_SFX(Sound_Hit);
		}
	}
}
void Boss::SetLimLightColor()
{
	if (HitFXStart == false)
	{
		//��Ų ������Ʈ ��������
		if (mSkinFilter == nullptr) { mSkinFilter = gameobject->GetChildMesh(1)->GetComponent<MeshFilter>(); }

		//���׸��� ��� ��������
		mSkinFilter->SetMaterialPropertyBlock(true);
		MPB = mSkinFilter->GetMaterialPropertyBlock();
		//�� ����Ʈ ���� ���� ����
		MPB->LimLightColor = NowLimLightColor;
		MPB->LimLightFactor = NowLimLightFactor;
		MPB->LimLightWidth = NowLimLightWidth;
		//���� ũ�� ����
		float Scale = NowHitMonsterScale + NowHitMonsterScale_F;
		mTransform->Scale = { Scale,Scale,Scale };

		HitFXStart = true;
	}

	//�ð��� ���� ������Ʈ ȿ���� �پ���
	if (MPB->LimLightFactor <= 0)
	{
		float Scale = NowHitMonsterScale + NowHitMonsterScale_F;
		mTransform->Scale.x = Scale;
		mTransform->Scale.y = Scale;
		mTransform->Scale.z = Scale;
		HitFunction = nullptr;
		HitFXStart = false;
		mSkinFilter->SetMaterialPropertyBlock(false);
	}
	else
	{
		float DTime = GetDeltaTime();
		mTransform->Scale.x -= DTime * NowHitMonsterScale_F;
		mTransform->Scale.y -= DTime * NowHitMonsterScale_F;
		mTransform->Scale.z -= DTime * NowHitMonsterScale_F;
		MPB->LimLightFactor -= DTime * NowLimLightFactor;
	}
}

void Boss::Hit()
{
	//���� ��������
	if (HP > 0)
	{
		//mAnimation->Choice(Animation_hit, Animation_hit_Speed);
		//float End = mAnimation->GetEndFrame();
		//float Now = mAnimation->GetNowFrame();
		HitStart = false;
		//if (Now >= End)
		//{
		//	SetMonsterState(MONSTER_STATE::CHASE);
		//}
	}
	else
	{
		//mAnimation->Choice(Animation_Die);
		//SetMonsterState(MONSTER_STATE::DEAD);
	}
}
