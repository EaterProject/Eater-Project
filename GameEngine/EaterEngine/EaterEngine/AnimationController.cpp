#include "AnimationController.h"
#include "Animator.h"
#include "GameObject.h"
#include "KeyinputManager.h"
#include "MeshFilter.h"
#include "Animation.h"
#include "TimeManager.h"

#include "Profiler/Profiler.h"

AnimationController::AnimationController()
{
	ChangeAnimation = false;

	IsBoneUpdate = false;

	mStop = false;
	mLoop = true;

	mNowFrame = 0;
	mNextFrame = 1;

	mPlayTime = 1.0f;
	mTime = 0.0f;
	mFrameTime = 0.0f;
}

AnimationController::~AnimationController()
{

}

void AnimationController::StartUpdate()
{
	
}

void AnimationController::Update()
{
	// ���� ������� ��쿡�� Play..
	if (mStop == true) return;
	if (mPlay == false)return;

	// ���� Animation Frame �缳��..
	AnimationFrameIndex();

	// Bone Transform Update ����..
	//if (IsBoneUpdate)
	{
		//Animator ���۳�Ʈ���� Play�Լ��� �����Ų��
		int Size = AnimatorList.size();
		for (int i = 0; i < Size; i++)
		{
			if (AnimatorList[i] == nullptr) { continue; }

			AnimatorList[i]->Play(mPrevFrame, mNextFrame, mFrameTime, mLoop);
		}
	}
}

void AnimationController::SetSkin(GameObject* obj)
{
	SkinObject.push_back(obj);
}

void AnimationController::SetBoneList(std::vector<GameObject*>* m_ObjList)
{
	//���ӿ�����Ʈ����Ʈ�� �ִϸ����� ����Ʈ�� ���������
	int AnimatorCount = (int)m_ObjList->size();
	AnimatorList.resize(AnimatorCount);
	for (int i = 0; i < AnimatorCount; i++)
	{
		//������Ʈ�� ���°��
		GameObject* obj = (*m_ObjList)[i];
		if (obj == nullptr) { continue; }

		//���۳�Ʈ�� ���°��
		Animator* anime = obj->GetComponent<Animator>();
		if (anime == nullptr) { continue; }

		AnimatorList[i] = anime;
	}
}

void AnimationController::SetAnimation(Animation* animation)
{
	if (animation == nullptr) return;

	// �ش� Animation ����..
	mAnimation = animation;

	// �ش� Animation Buffer ����..
	mAnimationData = new AnimationData();

	// �ش� Skin Mesh�� Animation Data ����..
	for (auto& skin : SkinObject)
	{
		skin->OneMeshData->Animation_Data = mAnimationData;
		skin->OneMeshData->Animation_Buffer = mAnimation->m_AnimationBuffer;
	}

	NowAnimationBuffer = mAnimation->m_AnimationBuffer;
}

void AnimationController::ChangeAnime()
{
	
	if (ChangeAnimation == true)
	{
		NowAnimation = mAnimation->m_AnimationData->AnimList[NowAnimationName];
		if (NowAnimation == nullptr) { return; }

		mAnimationData->PrevAnimationIndex = NowAnimationBuffer->AnimationOffset[NowAnimation->m_Index];
		mAnimationData->NextAnimationIndex = NowAnimationBuffer->AnimationOffset[NowAnimation->m_Index];

		// Animation Data �ʱ�ȭ..
		Reset();

		//���� �ִϸ��̼��� �־��ش�
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
		mTime += (mTimeManager->DeltaTime() * mSpeed);
		mFrameTime += (mTimeManager->DeltaTime() * mSpeed) / NowAnimation->m_TicksPerFrame;

		mNowFrame = (int)(mTime / NowAnimation->m_TicksPerFrame);

		if (mPrevFrame != mNowFrame)
		{
			mPrevFrame = mNowFrame;
			mFrameTime = 0.0f;

			if (mPrevFrame >= NowAnimation->m_EndFrame)
			{
				if (mLoop == false)
				{
					mPrevFrame = NowAnimation->m_EndFrame;
					mTime = 0.0f;
				}
				else
				{
					mPrevFrame = 0;
					mTime = 0.0f;
				}
			}

			mNextFrame = mPrevFrame + 1;

			if (mNextFrame >= NowAnimation->m_EndFrame)
			{
				if (mLoop == false)
				{
					mNextFrame = NowAnimation->m_EndFrame;
				}
				else
				{
					mNextFrame = 0;
				}
			}

			// �ش� Animation Frame ����..
			mAnimationData->PrevFrameIndex = mPrevFrame * NowAnimationBuffer->FrameOffset;
			mAnimationData->NextFrameIndex = mNextFrame * NowAnimationBuffer->FrameOffset;
		}

		// �ش� Animation Frame Time ����..
		mAnimationData->FrameTime = mFrameTime;
	}
}

void AnimationController::Reset()
{
	mPrevFrame = 0;
	mNextFrame = 1;

	mTime = 0.0f;
	mFrameTime = 0.0f;

	mAnimationData->PrevFrameIndex = mPrevFrame * NowAnimationBuffer->FrameOffset;
	mAnimationData->NextFrameIndex = mNextFrame * NowAnimationBuffer->FrameOffset;
}

void AnimationController::Choice(std::string Name, float Speed, bool Loop)
{
	//���� �ִϸ��̼� ����Ʈ���� ������ �ִϸ��̼��� ������ �Ѱ��ش�
	if (NowAnimationName != Name)
	{
		ChangeAnimation = true;
		NowAnimationName = Name;
		mLoop	= Loop;
		mSpeed	= Speed;
		ChangeAnime();
	}
}

void AnimationController::Play()
{
	mPlay = true;
	mStop = false;
}

void AnimationController::Stop()
{
	mPlay = false;
	//mStop = true;

	SetFrame(0);
}

void AnimationController::Pause()
{
	mPlay = false;
	mStop = true;
	mNowFrame = 0;
}

void AnimationController::SetFrame(int index)
{
	if (NowAnimation == nullptr) { return; }

	// ���� �������� �� ���� �ð� ����..
	mTime = index * NowAnimation->m_TicksPerFrame;

	// �ش� ������ �ε��� ����..
	mPrevFrame = index;

	if (mPrevFrame > NowAnimation->m_EndFrame)
	{
		mPrevFrame = 0;
	}

	mNextFrame = mPrevFrame + 1;

	if (mNextFrame > NowAnimation->m_EndFrame)
	{
		mNextFrame = 0;
	}

	mFrameTime = 0.0f;

	// �ִϸ��̼� ������ ����..
	mAnimationData->PrevFrameIndex = mPrevFrame * NowAnimationBuffer->FrameOffset;
	mAnimationData->NextFrameIndex = mNextFrame * NowAnimationBuffer->FrameOffset;
}

void AnimationController::SetIsBoneUpdate(bool enable)
{
	IsBoneUpdate = enable;
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

float AnimationController::GetFrameTime()
{
	return mTime;
}

int AnimationController::GetAnimationCount()
{
	if (mAnimation == nullptr)
	{
		return 0;
	}
	else
	{
		return (int)mAnimation->m_AnimationData->AnimList.size();
	}
}

void AnimationController::GetAnimationList(std::vector<std::string>* NameList)
{
	if (mAnimation == nullptr) { return; }
	auto it_Start = mAnimation->m_AnimationData->AnimList.begin();
	for (it_Start; it_Start != mAnimation->m_AnimationData->AnimList.end(); it_Start++)
	{
		NameList->push_back(it_Start->first);
	}
}

std::string AnimationController::GetNowAnimationName()
{
	return NowAnimationName;
}

bool AnimationController::EventCheck()
{
	if (NowAnimation == nullptr) { return false; }
	float Time = GetFrameTime();
	if (NowAnimation->m_Event_min <= Time && Time <= NowAnimation->m_Event_max)
	{
		return true;
	}
	else
	{
		return false;
	}
}
