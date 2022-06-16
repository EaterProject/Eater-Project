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
		//플레이어가 공격 상태일때
		if (Player::GetAttackState() == true)
		{
			MessageManager::GetGM()->SEND_Message(TARGET_PLAYER, MESSAGE_PLAYER_ATTACK_OK);
			//색을 바꾸는 함수포인터를 넣고 상태변화
			HitFunction = std::bind(&Boss::SetLimLightColor, this);
			SetMonsterState(MONSTER_STATE::HIT);
			Hit();
			HP -= 20;
			HitStart = true;
			//사운드 출력
			Sound_Play_SFX(Sound_Hit);
		}
	}
}
void Boss::SetLimLightColor()
{
	if (HitFXStart == false)
	{
		//스킨 오브젝트 가져오기
		if (mSkinFilter == nullptr) { mSkinFilter = gameobject->GetChildMesh(1)->GetComponent<MeshFilter>(); }

		//메테리얼 블록 가져오기
		mSkinFilter->SetMaterialPropertyBlock(true);
		MPB = mSkinFilter->GetMaterialPropertyBlock();
		//림 라이트 시작 설정 설정
		MPB->LimLightColor = NowLimLightColor;
		MPB->LimLightFactor = NowLimLightFactor;
		MPB->LimLightWidth = NowLimLightWidth;
		//몬스터 크기 설정
		float Scale = NowHitMonsterScale + NowHitMonsterScale_F;
		mTransform->Scale = { Scale,Scale,Scale };

		HitFXStart = true;
	}

	//시간에 따라 림라이트 효과가 줄어든다
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
	//공격 당했을때
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
