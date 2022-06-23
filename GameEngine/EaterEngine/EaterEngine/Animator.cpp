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
}

void Animator::SetAnimation(ParserData::CAnimation* data)
{
	NowAnimationData = data;
}

void Animator::Play(int prevIndex, int nextIndex, float time, bool Loop)
{
	if (NowAnimationData != nullptr)
	{
		//transfrom = gameobject->transform;
		//
		//DirectX::SimpleMath::Vector3	prevScl = NowAnimationData->m_AniData[prevIndex]->m_LocalScale;
		//DirectX::SimpleMath::Quaternion prevRot = NowAnimationData->m_AniData[prevIndex]->m_LocalRotQt;
		//DirectX::SimpleMath::Vector3	prevPos = NowAnimationData->m_AniData[prevIndex]->m_LocalPos;
		//
		//DirectX::SimpleMath::Vector3	nextScl = NowAnimationData->m_AniData[nextIndex]->m_LocalScale;
		//DirectX::SimpleMath::Quaternion nextRot = NowAnimationData->m_AniData[nextIndex]->m_LocalRotQt;
		//DirectX::SimpleMath::Vector3	nextPos = NowAnimationData->m_AniData[nextIndex]->m_LocalPos;
		//
		//DirectX::SimpleMath::Vector3	nowScl = Vector3::Lerp(prevScl, nextScl, time);
		//DirectX::SimpleMath::Quaternion nowRot = Quaternion::Slerp(prevRot, nextRot, time);
		//DirectX::SimpleMath::Vector3	nowPos = Vector3::Lerp(prevPos, nextPos, time);
		//
		//XM_SCL = DirectX::XMMatrixScaling(nowScl.x, nowScl.y, nowScl.z);
		//XM_ROT = DirectX::XMMatrixRotationQuaternion(nowRot);
		//XM_POS = DirectX::XMMatrixTranslation(nowPos.x, nowPos.y, nowPos.z);
		//
		//transfrom->SetLoadLocal(XM_SCL * XM_ROT * XM_POS);
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