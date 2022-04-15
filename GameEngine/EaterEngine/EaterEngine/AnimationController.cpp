#include "AnimationController.h"
#include "Animator.h"
#include "GameObject.h"
#include "KeyinputManager.h"
#include "MeshFilter.h"
#include "TimeManager.h"

AnimationController::AnimationController()
{
	ChangeAnimation = false;

	mStop = false;
	mLoop = true;

	mNowFrame = 0;
	mNextFrame = 1;

	mPlayTime = 1.0f;
	mTime = 0.0f;
}

AnimationController::~AnimationController()
{

}

void AnimationController::StartUpdate()
{
	ChangeAnime();
}

void AnimationController::Update()
{
	if (NowAnimationName != "" && AnimationList != nullptr)
	{
		Play(1, true);
	}
}

void AnimationController::SetBoneList(std::vector<GameObject*>* m_ObjList)
{
	//게임오브젝트리스트를 애니메이터 리스트로 변경시켜줌
	int AnimatorCount = (int)m_ObjList->size();
	AnimatorList.resize(AnimatorCount);
 	for (int i = 0; i < AnimatorCount; i++)
	{
		//오브젝트가 없는경우
		GameObject* obj = (*m_ObjList)[i];
		if (obj == nullptr) { continue; }

		//컨퍼넌트가 없는경우
		Animator* anime = obj->GetComponent<Animator>();
 		if (anime == nullptr) { continue; }

		AnimatorList[i] = anime;
	}
}

void AnimationController::SetAnimeList(ModelAnimationData* data)
{
	AnimationList = data;
}

void AnimationController::ChangeAnime()
{
	if (ChangeAnimation == true)
	{
		NowAnimation = AnimationList->AnimList[NowAnimationName];

		//본의 애니메이션을 넣어준다
		int Count = (int)AnimatorList.size();
		for (int i = 0; i < Count; i++)
		{
			if (AnimatorList[i] == nullptr) { continue; }

			AnimatorList[i]->SetAnimation(NowAnimation->m_AnimationList[i]);
		}
		ChangeAnimation = false;
	}
}

void AnimationController::AnimationFrameIndex()
{
	if (NowAnimation == nullptr) return;

	if (mStop == false)
	{
		mTime += (mTimeManager->DeltaTime() * mPlayTime);

		mNowFrame = (int)(mTime / NowAnimation->m_TicksPerFrame);

		if (mNowFrame != mNowFrame)
		{
			mNowFrame = mNowFrame;
		}
	}

	if (mNowFrame >= NowAnimation->m_EndFrame)
	{
		if (mLoop == false)
		{
			mNowFrame = NowAnimation->m_EndFrame;
			mTime = 0.0f;
		}
		else
		{
			mNowFrame = 0;
			mTime = 0.0f;
		}
	}
}

void AnimationController::Choice(std::string Name)
{
	//나의 애니메이션 리스트에서 선택한 애니메이션을 본에게 넘겨준다
	if (NowAnimationName != Name)
	{
		ChangeAnimation = true;
		NowAnimationName = Name;
	}
}

void AnimationController::Play(float Speed, bool Loop)
{
	// 현재 재생중일 경우에만 Play..
	if (mStop) return;

	// 현재 Animation Frame 재설정..
	AnimationFrameIndex();

	//Animator 컨퍼넌트들의 Play함수를 실행시킨다
	std::vector<Animator*>::iterator it = AnimatorList.begin();
	for (it; it != AnimatorList.end(); it++)
	{
		if ((*it) == nullptr) { continue; }

		(*it)->Play(mNowFrame, Speed, Loop);
	}
}

void AnimationController::Stop()
{
	mStop = true;
}

int AnimationController::GetNowFrame()
{
	if (NowAnimation == nullptr)
	{
		return 0;
	}

	return mNowFrame;
}

int AnimationController::GetEndFrame()
{
	if (NowAnimation == nullptr)
	{
		return 0;
	}

	return NowAnimation->m_EndFrame - 1;
}

int AnimationController::GetAnimationCount()
{
	if (AnimationList == nullptr)
	{
		return 0;
	}
	else
	{
		return (int)AnimationList->AnimList.size();
	}
}

void AnimationController::GetAnimationList(std::vector<std::string>* NameList)
{
	if (AnimationList == nullptr) { return; }
	std::map<std::string, CModelAnimation*>::iterator it_Start = AnimationList->AnimList.begin();
	for (it_Start; it_Start != AnimationList->AnimList.end(); it_Start++) 
	{
		NameList->push_back(it_Start->first);
	}

}

std::string AnimationController::GetNowAnimationName()
{
	return NowAnimationName;
}
