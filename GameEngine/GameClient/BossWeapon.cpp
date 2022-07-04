#include "BossWeapon.h"
#include "GameObject.h"
#include "MeshFilter.h"
#include "Transform.h"
#include "EaterEngineAPI.h"
#include "ParticleController.h"
#include "ParticleFactory.h"
#include "Player.h"
#include "MessageManager.h"
#include "ParticleFactory.h"
#include "ParticleSystem.h"

BossWeapon::BossWeapon()
{

}

BossWeapon::~BossWeapon()
{

}

void BossWeapon::Awake()
{
	mExplosion = ParticleFactory::Get()->CreateParticleController(PARTICLE_TYPE::BossProjectileExplode);
	mExplosionRange = ParticleFactory::Get()->CreateParticleController(PARTICLE_TYPE::BossProjectileFloor);
	mProjectile	= ParticleFactory::Get()->CreateParticleController(PARTICLE_TYPE::BossProjectile);


	mMeshFilter = gameobject->GetComponent<MeshFilter>();
	mTransform = gameobject->GetTransform();
}

void BossWeapon::SetUp()
{
	mMeshFilter->SetModelName("BossB_ball");
	mMF_Setting.Setting(this->gameobject);
	//gameobject->SetActive(false);
	StartPoint = { -44.0f,-10,62.0f };
}

void BossWeapon::Start()
{
	//mParticleManager->Play();
	//mParticleManager->gameobject->ChoiceParent(this->gameobject);

	//ParticleController* PC = nullptr;
	//PC =  mParticleObj[0]->GetComponent<ParticleController>();
	//PC->Play();
	//PC =  mParticleObj[1]->GetComponent<ParticleController>();
	//PC->Play();
	//PC =  mParticleObj[2]->GetComponent<ParticleController>();
	//PC->Play();
	mPlayerTr = Player::GetPlayerTransform();
}

void BossWeapon::Update()
{
	if (IsStart == false) { return; }

	if (IsSizeUpdate == true)
	{
		if (mTransform->GetScale().x >= UpScale)
		{
			IsSizeUpdate = false;
			IsShooting = true;
			IsReady = true;
			Sound_Play_SFX("Boss_ThrowWeapon");
		}
		else
		{
			float DTime = GetDeltaTime() * ScaleSpeed;
			mTransform->AddScale(DTime, DTime, DTime);
			IsReady = false;
			ParticleSize += DTime;
			mProjectile->SetScale(ParticleSize);
			mProjectile->SetPosition(mTransform->GetPosition());
		}
		mProjectile->Play();
	}
	else if (IsShooting == true)
	{
		if (mTransform->GetPosition().x >= End_Shooting_Pos.x - 0.4f && mTransform->GetPosition().x <= End_Shooting_Pos.x + 0.4f &&
			mTransform->GetPosition().y >= End_Shooting_Pos.y - 0.4f && mTransform->GetPosition().y <= End_Shooting_Pos.y + 0.4f &&
			mTransform->GetPosition().z >= End_Shooting_Pos.z - 0.4f && mTransform->GetPosition().z <= End_Shooting_Pos.z + 0.4f)
		{
			IsStart = false;
			if (IsGroundExplosion == true)
			{
				Explosion();
				IsGroundExplosion = false;
			}
			else
			{
				mExplosionRange->SetPosition(mTransform->GetPosition());
				mExplosionRange->Play();
			}
			mProjectile->Stop();
		}
		else
		{
			float DTime = GetDeltaTime();
			mTransform->AddPosition_X(ShootingNormalize.x * DTime * MoveSpeed);
			mTransform->AddPosition_Y(ShootingNormalize.y * DTime * MoveSpeed);
			mTransform->AddPosition_Z(ShootingNormalize.z * DTime * MoveSpeed);
			float Dir = mTransform->GetDistance(mPlayerTr->GetPosition());

			mProjectile->SetPosition(mTransform->GetPosition());
			if (Dir <= 1)
			{
				mExplosion->SetPosition(mTransform->GetPosition());
				mExplosion->Play();
				int Damage = 10;
				MessageManager::GetGM()->SEND_Message(TARGET_PLAYER, MESSAGE_PLAYER_HIT, &Damage);
				MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_PLAYER_HIT, nullptr);
				Reset();
			}
		}
	}

	mTransform->AddRotate_X(GetDeltaTime() * 500);
	//mMF_Setting.Update(1);
}

void BossWeapon::Debug()
{
	DebugDrawSphere(1, mTransform->GetPosition(), Vector3(0,0,255));
	DebugDrawSphere(2.8f, mTransform->GetPosition(), Vector3(0,0,255));
}


void BossWeapon::SetScale(float UpSize)
{
	UpScale =UpSize;
	ParticleSize= UpScale;
}

void BossWeapon::SetShootingPoistion(Vector3 Start, Vector3 End, float mScaleSpeed, float mMoveSpeed,bool GroundExplosion)
{
	//mMF_Setting.SetLimlightSetting(0, 0, 0, 1, 1);
	//mMF_Setting.SetLimlightSettingMax(1, 0, 0, 1, 1);

	gameobject->SetActive(true);
	//지점 설정
	Start_Shooting_Pos = Start;
	End_Shooting_Pos = End;

	//현재 지점 설정
	mTransform->SetPosition(Start);
	mTransform->SetScale(1, 1, 1);

	//방향벡터 구하기
	ShootingNormalize = End - Start;
	ShootingNormalize.Normalize();

	IsGroundExplosion = GroundExplosion;
	IsSizeUpdate = true;
	IsStart = true;
	ScaleSpeed = mScaleSpeed;
	MoveSpeed = mMoveSpeed;
	Sound_Play_SFX("Boss_SpawnWeapon");
}

void BossWeapon::Reset()
{
	mTransform->SetPosition(StartPoint);
	mTransform->SetScale(1,1, 1);
	IsShooting = false;
	IsSizeUpdate = false;
	IsReady = false;
	IsStart = false;
	ParticleSize = 0.0f;
}

bool BossWeapon::ShootingReady()
{
	return IsReady;
}

void BossWeapon::Explosion()
{
	float Dir = mTransform->GetDistance(mPlayerTr->GetPosition());
	if (Dir <= 2.8f)
	{
		int Damage = 10;
		MessageManager::GetGM()->SEND_Message(TARGET_PLAYER, MESSAGE_PLAYER_HIT, &Damage);
		MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_PLAYER_HIT, nullptr);
	}
	mExplosion->SetPosition(mTransform->GetPosition());
	mExplosion->Play();

	mExplosionRange->SetPosition(mTransform->GetPosition());
	mExplosionRange->Stop();
	
	Sound_Play_SFX("Boss_WeaponExplosion");
	Reset();
}

