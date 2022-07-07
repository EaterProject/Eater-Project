#include "BossFriend.h"
#include "MeshFilter.h"
#include "Transform.h"
#include "GameObject.h"
#include "AnimationController.h"
#include "Player.h"

BossFriend::BossFriend()
{
}

BossFriend::~BossFriend()
{


}

void BossFriend::Awake()
{
	mTransform	= gameobject->GetTransform();
	mMeshFilter = gameobject->GetComponent<MeshFilter>();
	mAnimation	= gameobject->GetComponent<AnimationController>();
	mTransform->SetScale(1.5f, 1.5f, 1.5f);
	StartPos = { -44.0f,-10,62.0f };
}

void BossFriend::SetUp()
{
	mMeshFilter->SetModelName("BossB+");
	mMeshFilter->SetAnimationName("BossB+");
	gameobject->SetActive(false);
}

void BossFriend::Start()
{
	mSetting.Setting(this->gameobject);
	mSetting.SetDissolveOption(DISSOLVE_FADEIN);
	mSetting.SetDissolveTexture("Dissolve_16");
	mSetting.SetDissolveColor(255.0f, 0, 0);
	mSetting.SetDissolveColorFactor(10.0f);
	mSetting.SetDissolvePlayTime(3.0f);
	mSetting.SetDissolveWidth(0.75f);
	mSetting.SetDissolveInnerFactor(69.83);
	mSetting.SetDissolveOuterFactor(9.49);

	//mSetting.SetLimlightSetting(0.1f, 0.1f, 0.1f, 1.25f, 2.25f);

	mTransform->SetPosition(StartPos);
}

void BossFriend::Update()
{
	mTransform->Slow_Y_Rotation(Player::GetPlayerTransform()->GetPosition(), 150);
	//mAnimation->Choice("idle");
	//mAnimation->Play();
}

void BossFriend::PlayDissolve(float playtime, DISSOLVE_OPTION option)
{
	mSetting.SetDissolveOption(option);
	mSetting.SetDissolvePlayTime(playtime);
	//mSetting.LimLightUpdate(0.0f);
	mSetting.PlayDissolve();
}

void BossFriend::SetPosition(Vector3 Pos)
{
	gameobject->SetActive(true);
	mTransform->SetPosition(Pos);
}

void BossFriend::SetPosition(float x, float y, float z)
{
	gameobject->SetActive(true);
	mTransform->SetPosition(x, y, z);
}

void BossFriend::Reset()
{
	mTransform->SetPosition(StartPos);
}
