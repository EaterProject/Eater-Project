#include "Animator.h"
#include "ParserData.h"
#include "GameObject.h"
#include "Transform.h"
#include "TimeManager.h"
#include "KeyinputManager.h"

Animator::Animator()
{
	NowAnimationData = nullptr;
	transfrom = nullptr;

	XM_ROT = XMMatrixIdentity();
	XM_POS = XMMatrixIdentity();
	XM_SCL = XMMatrixIdentity();
}

Animator::~Animator()
{
	NowAnimationData = nullptr;
	AnimeData.clear();
}

void Animator::SetAnimation(ParserData::CAnimation* data)
{
	NowAnimationData = data;
}

void Animator::Play(int animeIndex, float time, bool Loop)
{
	if (NowAnimationData != nullptr)
	{
		transfrom = gameobject->transform;

		DirectX::SimpleMath::Vector3	pos = NowAnimationData->m_AniData[animeIndex]->m_LocalPos;
		DirectX::SimpleMath::Quaternion rot = NowAnimationData->m_AniData[animeIndex]->m_LocalRotQt;
		DirectX::SimpleMath::Vector3	scl = NowAnimationData->m_AniData[animeIndex]->m_LocalScale;

		XM_ROT = DirectX::XMMatrixRotationQuaternion(rot);
		XM_POS = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
		XM_SCL = DirectX::XMMatrixScaling(scl.x, scl.y, scl.z);

		transfrom->Load_Local = XM_SCL * XM_ROT * XM_POS;
	}
}

void Animator::ChoiceAnime(ParserData::CAnimation* Anime)
{
	NowAnimationData = Anime;
}

float Animator::GetOnePlayTime(float mPlayTime, int EndFrameCount)
{
	return (mPlayTime / EndFrameCount);
}