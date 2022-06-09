#pragma once
#include "Component.h"
#include "Utility.h"

class Image;
class RectTransform;

class Button : public Component
{
public:
	EATER_ENGINEDLL Button();
	virtual ~Button();

public:
	enum State
	{
		OUT_BUTTON,			// ��ư ������ ���콺�� ���� ���
		IN_BUTTON,			// ��ư ������ ���콺�� �ִ� ���
		DOWN_BUTTON,		// ��ư�� ������ ���
		UP_BUTTON,			// ��ư�� ���� ���
		PRESS_DOWN_BUTTON,	// ��ư�� ������ ���� ���
	};

public:
	void Awake() override;
	void Start() override;
	void Update() override;

public:
	EATER_ENGINEDLL void PushEvent(std::function<void()>& eventFunc, State type);
	EATER_ENGINEDLL void PopEvent(std::function<void()>& eventFunc, State type);
	EATER_ENGINEDLL void ResetEvent(State type);
	EATER_ENGINEDLL void ResetAllEvent();

private:
	Eater::Delegate<> m_OutButtonEvent;
	Eater::Delegate<> m_InButtonEvent;
	Eater::Delegate<> m_DownButtonEvent;
	Eater::Delegate<> m_UpButtonEvent;
	Eater::Delegate<> m_PressDownButtonEvent;

	State m_State;

	Image* m_Image;
	RectTransform* m_Transform;
};

