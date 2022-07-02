#pragma once
#include "GameObject.h"
#include <string>
//���� ���� �������̽�
class Transform;
class AnimationController;
class State
{
public:
	State() = default;
	virtual ~State() = default;
public:
	//���¸� �����ų ���� ������Ʈ�� �־��ش�,�׸��� �ʿ��� ������Ʈ�� �������� �Լ�
	virtual void SetObject(GameObject* Object);
	virtual int Play() = 0;

	//���� ���¿� �޼����� �ְ������(������ ������ ����, ������ �¾����� ���...)
	virtual void SetMessage(void* Message) {};

	//Ÿ�� ������Ʈ
	void SetTarget(GameObject* mTarget);
	void SetTarget(Transform* mTarget);

	//�ִϸ��̼��� �̸��� �־��ش�
	void SetAnimationOption(std::string& Name,float Speed = 1.0f);
	void SetAnimationOption(std::string&& Name, float Speed = 1.0f);
protected:
	GameObject* mObject				= nullptr;			//���� ���¸� �����ų ������Ʈ
	AnimationController* mAnimation = nullptr;			//���� ������Ʈ�� �ִϸ��̼�
	Transform*	mTransform			= nullptr;			//���� ������Ʈ�� ��ġ
	Transform*	mTargetTR			= nullptr;			//Ÿ���� ��ġ

	//���¿� �ִϸ��̼��� ������Ѿ� �ϴ� ��Ȳ�� �����ֱ⶧����
	std::string AnimationName	= "";				//���� ������ �ִϸ��̼� �̸�
	float AnimationSpeed		= 1.0f;				//���� ������ �ִϸ��̼� �ӵ�
};

