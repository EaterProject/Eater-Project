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

	mTime		= 0;
	PlayTime	= 1;
	AnimeIndex	= 0;

	mLoop = true;
	mStop = false;
	mPlay = false;
}

Animator::~Animator()
{
	NowAnimationData = nullptr;
	AnimeData.clear();
}

void Animator::SetAnimation(ParserData::CAnimation* data)
{
	NowAnimationData = data;
	AnimeIndex = 0;
}

void Animator::Play(float time, bool Loop)
{
	PlayTime	= time;
	mLoop		= Loop;
	mPlay		= true;


	if (NowAnimationData != nullptr && mPlay == true)
	{
		transfrom = gameobject->transform;
		AnimeFrameIndex();


		DirectX::SimpleMath::Vector3	pos = NowAnimationData->m_AniData[AnimeIndex]->m_LocalPos;
		DirectX::SimpleMath::Quaternion rot = NowAnimationData->m_AniData[AnimeIndex]->m_LocalRotQt;
		DirectX::SimpleMath::Vector3	scl = NowAnimationData->m_AniData[AnimeIndex]->m_LocalScale;

		XM_ROT = DirectX::XMMatrixRotationQuaternion(rot);
		XM_POS = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
		XM_SCL = DirectX::XMMatrixScaling(scl.x, scl.y, scl.z);

		transfrom->Load_Local = XM_SCL * XM_ROT * XM_POS;
	}
}

void Animator::Stop()
{
	mStop = true;
}

void Animator::ReStart()
{
	mStop = false;
}

void Animator::ChoiceAnime(ParserData::CAnimation* Anime)
{
	NowAnimationData = Anime;
	AnimeIndex = 0;
}

int Animator::GetNowFrame()
{
	return AnimeIndex;
}

int Animator::GetEndFrame()
{
	return NowAnimationData->m_EndFrame;
}

float Animator::GetOnePlayTime(float mPlayTime, int EndFrameCount)
{
	return (mPlayTime / EndFrameCount);
}

void Animator::AnimeFrameIndex()
{
	if (mStop == false)
	{
		mTime += (mTimeManager->DeltaTime() * PlayTime);

		int NextIndex = (int)(mTime / NowAnimationData->m_TicksPerFrame);

		if (NextIndex != AnimeIndex)
		{
			AnimeIndex = NextIndex;
		}
	}

	if (AnimeIndex >= NowAnimationData->m_EndFrame)
	{
		if (mLoop == false)
		{
			AnimeIndex = NowAnimationData->m_EndFrame;
			mTime = 0.0f;
		}
		else
		{
			AnimeIndex = 0;
			mTime = 0.0f;
		}
	}
}

