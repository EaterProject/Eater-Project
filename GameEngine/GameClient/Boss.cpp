#include "Boss.h"
#include "Collider.h"
#include "Rigidbody.h"
#include "MeshFilter.h"
#include "AnimationController.h"
#include "Transform.h"
#include "Player.h"
#include "MessageManager.h"
#include <functional>
#include "EaterEngineAPI.h"

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

	HP = 100;
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
			HitFunction = std::bind(&MonsterComponent::SetLimLightColor, this);
			SetMonsterState(MONSTER_STATE::HIT);

			HP -= 20;
			HitStart = true;
			//���� ���
			Sound_Play_SFX(Sound_Hit);
		}
	}
}
