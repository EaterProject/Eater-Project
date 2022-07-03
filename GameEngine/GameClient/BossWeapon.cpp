#include "BossWeapon.h"
#include "GameObject.h"
#include "MeshFilter.h"
#include "Transform.h"
#include "EaterEngineAPI.h"
#include "ParticleController.h"
#include "ParticleFactory.h"
BossWeapon::BossWeapon()
{

}

BossWeapon::~BossWeapon()
{

}

void BossWeapon::Awake()
{
	//mParticleObj[0] = Instance_Particle("Particle1", "BossProjectile_aura");
	//mParticleObj[1] = Instance_Particle("Particle2", "BossProjectile_circle");
	//mParticleObj[2] = Instance_Particle("Particle3", "BossProjectile_dot");

	//mParticleManager = ParticleFactory::Get()->CreateParticleController(PARTICLE_TYPE::BossProjectile);


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
}

void BossWeapon::Update()
{
	if (IsStart == false) { return; }

	if (IsSizeUpdate == true)
	{
		if (mTransform->GetScale().x >= 5.0f)
		{
			IsSizeUpdate = false;
			IsShooting = true;
			IsReady = true;
		}
		else
		{
			float DTime = GetDeltaTime() * ScaleSpeed;
			mTransform->AddScale(DTime, DTime, DTime);
			IsReady = false;
		}
	}
	else if (IsShooting == true)
	{
		if (mTransform->GetPosition().x >= End_Shooting_Pos.x - 0.4f && mTransform->GetPosition().x <= End_Shooting_Pos.x + 0.4f &&
			mTransform->GetPosition().y >= End_Shooting_Pos.y - 0.4f && mTransform->GetPosition().y <= End_Shooting_Pos.y + 0.4f &&
			mTransform->GetPosition().z >= End_Shooting_Pos.z - 0.4f && mTransform->GetPosition().z <= End_Shooting_Pos.z + 0.4f)
		{
			IsStart = false;
			//mParticleManager->Stop();
		}
		else
		{
			float DTime = GetDeltaTime();
			mTransform->AddPosition_X(ShootingNormalize.x * DTime * MoveSpeed);
			mTransform->AddPosition_Y(ShootingNormalize.y * DTime * MoveSpeed);
			mTransform->AddPosition_Z(ShootingNormalize.z * DTime * MoveSpeed);
		}
	}

	mTransform->AddRotate_X(GetDeltaTime() * 500);
	//mMF_Setting.Update(1);
}


void BossWeapon::SetShootingPoistion(Vector3 Start, Vector3 End, float mScaleSpeed, float mMoveSpeed)
{
	//mMF_Setting.SetLimlightSetting(0, 0, 0, 1, 1);
	//mMF_Setting.SetLimlightSettingMax(1, 0, 0, 1, 1);

	gameobject->SetActive(true);
	//지점 설정
	Start_Shooting_Pos = Start;
	End_Shooting_Pos = End;

	//현재 지점 설정
	mTransform->SetPosition(Start);
	mTransform->SetScale(0.0f, 0.0f, 0.0f);

	//방향벡터 구하기
	ShootingNormalize = End - Start;
	ShootingNormalize.Normalize();

	IsSizeUpdate = true;
	IsStart = true;
	ScaleSpeed = mScaleSpeed;
	MoveSpeed = mMoveSpeed;
}

void BossWeapon::Reset()
{
	mTransform->SetPosition(StartPoint);
	mTransform->SetScale(0.0f, 0.0f, 0.0f);
	IsShooting = false;
	IsSizeUpdate = false;
	IsReady = false;
	IsStart = false;
}

bool BossWeapon::ShootingReady()
{
	return IsReady;
}

void BossWeapon::RandomUpdate()
{




}
