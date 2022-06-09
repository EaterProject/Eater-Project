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
		OUT_BUTTON,			// 버튼 범위에 마우스가 없는 경우
		IN_BUTTON,			// 버튼 범위에 마우스가 있는 경우
		DOWN_BUTTON,		// 버튼을 눌렀을 경우
		UP_BUTTON,			// 버튼을 뗏을 경우
		PRESS_DOWN_BUTTON,	// 버튼을 누르고 있을 경우
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

