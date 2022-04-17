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

	EATER_ENGINEDLL void Choice(std::string Name);		//����
	EATER_ENGINEDLL void Play(float Speed, bool Loop);	//���
	EATER_ENGINEDLL void Stop();						//�Ͻ�����

	EATER_ENGINEDLL int GetNowFrame();	//������ �ִϸ��̼� ������
	EATER_ENGINEDLL int GetEndFrame();	//������ �ִϸ��̼� �� ������

	EATER_ENGINEDLL int GetAnimationCount();
	EATER_ENGINEDLL void GetAnimationList(std::vector<std::string>* NameList);
	EATER_ENGINEDLL std::string GetNowAnimationName();

	void SetSkin(GameObject* obj);
	void SetBoneList(std::vector<GameObject*>* mobjList);
	void SetAnimation(Animation* animation);

private:
	void ChangeAnime();
	void AnimationFrameIndex();

	//������ �ִϸ����� ����Ʈ
	std::vector<Animator*> AnimatorList;

	//��Ų ���� ������Ʈ
	GameObject* SkinObject;

	//�Ѱ��� �𵨿� ����ִ� �ִϸ��̼� ����Ʈ
	Animation* mAnimation;

	AnimationBuffer* NowAnimationBuffer;
	AnimationData* mAnimationData;

	ParserData::CModelAnimation* NowAnimation;

	//���� �ִϸ��̼� �̸�
	std::string NowAnimationName;

	bool ChangeAnimation;

	bool	mStop;
	bool	mLoop;

	int		mPrevFrame;
	int		mNowFrame;
	int		mNextFrame;

	float	mPlayTime;		//�ִϸ��̼��� �ѹ��� ���� �ð� 
	float 	mTime;			//���� ���Ÿ��
	float 	mFrameTime;		//���� ������ ���Ÿ��
};

