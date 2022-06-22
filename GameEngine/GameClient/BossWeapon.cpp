#include "BossWeapon.h"
#include "GameObject.h"
#include "MeshFilter.h"
#include "Transform.h"
#include "EaterEngineAPI.h"
BossWeapon::BossWeapon()
{

}

BossWeapon::~BossWeapon()
{

}

void BossWeapon::Awake()
{
	mMeshFilter = gameobject->GetComponent<MeshFilter>();
	mTransform	= gameobject->GetTransform();
}

void BossWeapon::SetUp()
{
	mMeshFilter->SetModelName("Sphere");
	//gameobject->SetActive(false);
	//mColor.Setting(this->gameobject);
}

void BossWeapon::Update()
{
	if (IsStart == false) { return; }

	if (IsSizeUpdate == true)
	{
		if (mTransform->GetScale().x >= 1.0f)
		{
			mTransform->SetScale(1.0f, 1.0f, 1.0f);
			IsSizeUpdate = false;
			IsShooting = true;
			IsReady = true;
		}
		else
		{
			float DTime = GetDeltaTime() * 2;
			mTransform->AddScale(DTime, DTime, DTime);
			IsReady = false;
		}
	}
	else if (IsShooting == true)
	{

		if (mTransform->GetPosition().x >= End_Shooting_Pos.x - 0.25f && mTransform->GetPosition().x <= End_Shooting_Pos.x + 0.25f &&
			mTransform->GetPosition().y >= End_Shooting_Pos.y - 0.25f && mTransform->GetPosition().y <= End_Shooting_Pos.y + 0.25f &&
			mTransform->GetPosition().z >= End_Shooting_Pos.z - 0.25f && mTransform->GetPosition().z <= End_Shooting_Pos.z + 0.25f)
		{
			IsStart = false;
		}
		else
		{
			float DTime = GetDeltaTime();
			mTransform->AddPosition_X(ShootingNormalize.x * DTime * Speed);
			mTransform->AddPosition_Y(ShootingNormalize.y * DTime * Speed);
			mTransform->AddPosition_Z(ShootingNormalize.z * DTime * Speed);
		}
	}

	//mColor.Update(1);
}


void BossWeapon::SetShootingPoistion(Vector3 Start, Vector3 End)
{
	//mColor.SetLimlightSetting(0, 0, 0, 1, 1);
	//mColor.SetLimlightSettingMax(1, 0, 0, 1, 1);

	gameobject->SetActive(true);
	//지점 설정
	Start_Shooting_Pos	= Start;
	End_Shooting_Pos	= End;

	//현재 지점 설정
	mTransform->SetPosition(Start);
	mTransform->SetScale(0.0f, 0.0f, 0.0f);
	
	//방향벡터 구하기
	ShootingNormalize = End - Start;
	ShootingNormalize.Normalize();

	IsSizeUpdate	= true;
	IsStart			= true;
}

void BossWeapon::Reset()
{
	mTransform->SetPosition(Start_Shooting_Pos);
	mTransform->SetScale(0.0f, 0.0f, 0.0f);
	IsShooting		= false;
	IsSizeUpdate	= false;
	IsReady			= false;
	IsStart			= false;
	gameobject->SetActive(false);
}

bool BossWeapon::ShootingReady()
{
	return IsReady;
}

void BossWeapon::RandomUpdate()
{




}
