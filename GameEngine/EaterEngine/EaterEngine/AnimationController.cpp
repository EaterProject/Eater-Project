#include "AnimationController.h"
#include "Animator.h"
#include "GameObject.h"
#include "KeyinputManager.h"
#include "MeshFilter.h"

AnimationController::AnimationController()
{
	ChangeAnimation = false;
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

void AnimationController::SetAnimeList(ModelAnimationData* data)
{
	AnimationList = data;
}

void AnimationController::ChangeAnime()
{
	if (ChangeAnimation == true)
	{
		std::vector<OneAnimation*>* data = AnimationList->AnimList[NowAnimationName];

		//���� �ִϸ��̼��� �־��ش�
		int Count = (int)AnimatorList.size();
		for (int i = 0; i < Count; i++)
		{
			if (AnimatorList[i] == nullptr) { continue; }

			AnimatorList[i]->SetAnimation((*data)[i]);
		}
		ChangeAnimation = false;
	}
}

void AnimationController::Choice(std::string Name)
{
	//���� �ִϸ��̼� ����Ʈ���� ������ �ִϸ��̼��� ������ �Ѱ��ش�
	if (NowAnimationName != Name)
	{
		ChangeAnimation = true;
		NowAnimationName = Name;
	}
}

void AnimationController::Play(float Speed, bool Loop)
{
	//Animator ���۳�Ʈ���� Play�Լ��� �����Ų��
	std::vector<Animator*>::iterator it = AnimatorList.begin();
	for (it; it != AnimatorList.end(); it++)
	{
		if ((*it) == nullptr) { continue; }

		(*it)->Play(Speed, Loop);
	}
}

void AnimationController::Stop()
{
	//Animator ���۳�Ʈ���� Play�Լ��� �����Ų��
	std::vector<Animator*>::iterator it = AnimatorList.begin();
	for (it; it != AnimatorList.end(); it++)
	{
		if ((*it) == nullptr) { continue; }
		(*it)->Stop();
	}
}

int AnimationController::GetNowFrame()
{
	if (AnimatorList[0] == nullptr)
	{
		return 0;
	}
	return AnimatorList[0]->GetNowFrame();
}

int AnimationController::GetEndFrame()
{
	if (AnimatorList[0] == nullptr)
	{
		return 0;
	}
	return AnimatorList[0]->GetEndFrame()-1;
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
	std::map<std::string, std::vector<OneAnimation*>*>::iterator it_Start = AnimationList->AnimList.begin();
	for (it_Start; it_Start != AnimationList->AnimList.end(); it_Start++) 
	{
		NameList->push_back(it_Start->first);
	}

}

std::string AnimationController::GetNowAnimationName()
{
	return NowAnimationName;
}
