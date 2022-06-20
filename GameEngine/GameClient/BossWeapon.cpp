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
}

void BossWeapon::Update()
{
	if (IsStart == false) { return; }

	if (IsSizeUpdate == true)
	{
		if (mTransform->Scale.x >= 1.0f)
		{
			mTransform->Scale.x = 1.0f;
			mTransform->Scale.y = 1.0f;
			mTransform->Scale.z = 1.0f;
			IsSizeUpdate = false;
			IsShooting	= true;
			IsReady		= true;
		}
		else
		{
			float DTime = GetDeltaTime();
			mTransform->Scale.x += DTime;
			mTransform->Scale.y += DTime;
			mTransform->Scale.z += DTime;
			IsReady = false;
		}
	}
	else if (IsShooting == true)
	{
	
		if (mTransform->Position.x  >= End_Shooting_Pos.x -0.1f && mTransform->Position.x <= End_Shooting_Pos.x + 0.1f &&
			mTransform->Position.y >= End_Shooting_Pos.y - 0.1f && mTransform->Position.y <= End_Shooting_Pos.y + 0.1f &&
			mTransform->Position.z >= End_Shooting_Pos.z - 0.1f && mTransform->Position.z <= End_Shooting_Pos.z + 0.1f)
		{
			IsStart = false;
		}
		else
		{
			float DTime = GetDeltaTime();
			mTransform->Position.x += ShootingNormalize.x * DTime * Speed;
			mTransform->Position.y += ShootingNormalize.y * DTime * Speed;
			mTransform->Position.z += ShootingNormalize.z * DTime * Speed;
		}
	}
}


void BossWeapon::SetShootingPoistion(Vector3 Start, Vector3 End)
{
	gameobject->SetActive(true);
	//지점 설정
	Start_Shooting_Pos	= Start;
	End_Shooting_Pos	= End;

	//현재 지점 설정
	mTransform->Position = Start;
	mTransform->Scale = { 0.0f,0.0f, 0.0f, };
	
	//방향벡터 구하기
	ShootingNormalize = End - Start;
	ShootingNormalize.Normalize();

	IsSizeUpdate	= true;
	IsStart			= true;
}

void BossWeapon::Reset()
{
	mTransform->Position = Start_Shooting_Pos;
	mTransform->Scale = { 0.0f, 0.0f, 0.0f };
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
