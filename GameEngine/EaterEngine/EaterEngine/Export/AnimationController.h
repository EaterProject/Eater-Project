#pragma once
#include "Component.h"
#include "EaterEngineDLL.h"
#include <vector>
#include <string>

/// <summary>
/// Animator���� �����ϴ� Ŭ����
/// �ֻ��� ������Ʈ���� �����ɰ�
/// </summary>

namespace ParserData
{
	class CModelAnimation;
}

class AnimationBuffer;
class AnimationData;
class Animation;
class Animator;
class AnimationController : public Component
{
public:
	EATER_ENGINEDLL AnimationController();
	EATER_ENGINEDLL virtual ~AnimationController();

	virtual void StartUpdate();
	virtual void Update();

	EATER_ENGINEDLL void Choice(std::string Name,float Speed =1 ,bool Loop = true);//����
	
	EATER_ENGINEDLL void Play();	//���
	EATER_ENGINEDLL void Stop();	//����
	EATER_ENGINEDLL void Pause();	//����
	EATER_ENGINEDLL void SetFrame(int index);

	EATER_ENGINEDLL int GetNowFrame();					//������ �ִϸ��̼� ������
	EATER_ENGINEDLL int GetEndFrame();					//������ �ִϸ��̼� �� ������
	EATER_ENGINEDLL float GetFrameTime();				//������ �ִϸ��̼� �ð�
	EATER_ENGINEDLL int GetAnimationCount();
	EATER_ENGINEDLL void GetAnimationList(std::vector<std::string>* NameList);
	EATER_ENGINEDLL std::string GetNowAnimationName();	//���� �ִϸ��̼��� �̸�
	EATER_ENGINEDLL bool EventCheck();	//���� �ִϸ��̼��� �̸�


	void SetSkin(GameObject* obj);
	void SetBoneList(std::vector<GameObject*>* mobjList);
	void SetAnimation(Animation* animation);

private:
	void ChangeAnime();
	void AnimationFrameIndex();
	void Reset();

	//������ �ִϸ����� ����Ʈ
	std::vector<Animator*> AnimatorList;

	//��Ų ���� ������Ʈ
	std::vector<GameObject*> SkinObject;

	//�Ѱ��� �𵨿� ����ִ� �ִϸ��̼� ����Ʈ
	Animation* mAnimation;

	AnimationBuffer* NowAnimationBuffer;
	AnimationBuffer* NextAnimationBuffer;
	AnimationData* mAnimationData;

	ParserData::CModelAnimation* NowAnimation;

	//���� �ִϸ��̼� �̸�
	std::string NowAnimationName;

	bool ChangeAnimation;

	bool	mStop;
	bool	mLoop;
	bool	mPlay;

	int		mPrevFrame;
	int		mNowFrame;
	int		mNextFrame;

	float	mSpeed;
	float	mPlayTime;		//�ִϸ��̼��� �ѹ��� ���� �ð� 
	float 	mTime;			//���� ���Ÿ��
	float 	mFrameTime;		//���� ������ ���Ÿ��
};

