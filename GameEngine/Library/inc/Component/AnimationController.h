#pragma once
#include "Component.h"
#include "EaterEngineDLL.h"
#include <vector>
#include <string>

/// <summary>
/// Animator���� �����ϴ� Ŭ����
/// �ֻ��� ������Ʈ���� �����ɰ�
/// </summary>

class ModelAnimationData;
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


	void SetBoneList(std::vector<GameObject*>* mobjList);
	void SetAnimeList(ModelAnimationData* data);
private:
	void ChangeAnime();

	//������ �ִϸ����� ����Ʈ
	std::vector<Animator*> AnimatorList;

	//�Ѱ��� �𵨿� ����ִ� �ִϸ��̼� ����Ʈ
	ModelAnimationData* AnimationList;

	//���� �ִϸ��̼� �̸�
	std::string NowAnimationName;

	bool ChangeAnimation;

};

