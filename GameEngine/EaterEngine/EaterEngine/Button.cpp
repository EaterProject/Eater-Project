#include "Button.h"
#include "GameObject.h"
#include "RectTransform.h"
#include "Image.h"
#include "KeyinputManager.h"

#include "Profiler/Profiler.h"

Button::Button()
	:m_State(OUT_BUTTON)
{

}

Button::~Button()
{

}

void Button::Awake()
{

}

void Button::Start()
{
	m_Image = gameobject->GetComponent<Image>();
	m_Transform = gameobject->GetComponent<RectTransform>();
	m_Rect = m_Transform->GetRectPoint();
}

void Button::Update()
{
	const LPPOINT& mouse_point = mKeyInputManger->GetClientMousePos();

	//PROFILE_LOG(PROFILE_OUTPUT::VS_CODE, "Mouse Point : %d, %d", mouse_point->x, mouse_point->y);

	if (m_Rect->left < mouse_point->x && m_Rect->right > mouse_point->x)
	{
		if (m_Rect->top < mouse_point->y && m_Rect->bottom > mouse_point->y)
		{
			// 마우스가 버튼 위에 있는 경우..
			if (m_State == OUT_BUTTON)
			{
				m_State = IN_BUTTON;

				// 등록된 이벤트 실행..
				m_InButtonEvent();

				//PROFILE_LOG(PROFILE_OUTPUT::VS_CODE, "State : OUT_BUTTON -> IN_BUTTON");
			}

			if (mKeyInputManger->GetKey(VK_LBUTTON))
			{
				// 마우스를 누른 상태인 경우..
				switch (m_State)
				{
				case Button::DOWN_BUTTON:
				{
					m_State = PRESS_DOWN_BUTTON;

					// 등록된 이벤트 실행..
					m_PressDownButtonEvent();

					//PROFILE_LOG(PROFILE_OUTPUT::VS_CODE, "State : DOWN_BUTTON -> PRESS_DOWN_BUTTON");
				}
					break;
				case Button::IN_BUTTON:
				case Button::UP_BUTTON:
				{
					m_State = DOWN_BUTTON;

					// 등록된 이벤트 실행..
					m_DownButtonEvent();

					//PROFILE_LOG(PROFILE_OUTPUT::VS_CODE, "State : UP_BUTTON -> DOWN_BUTTON");
				}
					break;
				case Button::PRESS_DOWN_BUTTON:
				{
					// 등록된 이벤트 실행..
					m_PressDownButtonEvent();

					//PROFILE_LOG(PROFILE_OUTPUT::VS_CODE, "State : PRESS_DOWN_BUTTON -> PRESS_DOWN_BUTTON");
				}
					break;
				default:
					break;
				}
			}
			else
			{
				// 마우스를 뗀 상태인 경우..
				switch (m_State)
				{
				case Button::DOWN_BUTTON:
				{
					m_State = UP_BUTTON;

					// 등록된 이벤트 실행..
					m_UpButtonEvent();

					//PROFILE_LOG(PROFILE_OUTPUT::VS_CODE, "State : DOWN_BUTTON -> UP_BUTTON");
				}
				break;
				case Button::PRESS_DOWN_BUTTON:
				{
					m_State = UP_BUTTON;

					// 등록된 이벤트 실행..
					m_UpButtonEvent();

					//PROFILE_LOG(PROFILE_OUTPUT::VS_CODE, "State : PRESS_DOWN_BUTTON -> UP_BUTTON");
				}
				break;
				default:
					break;
				}
			}
		}
	}
	else if (m_State != OUT_BUTTON)
	{
		switch (m_State)
		{
		case Button::DOWN_BUTTON:
		case Button::PRESS_DOWN_BUTTON:
		{
			// 등록된 이벤트 실행..
			m_UpButtonEvent();
		}
			break;
		default:
			break;
		}

		m_State = OUT_BUTTON;

		// 등록된 이벤트 실행..
		m_OutButtonEvent();
	}
}

void Button::PushEvent(std::function<void()>& eventFunc, State type)
{
	switch (type)
	{
	case Button::OUT_BUTTON:
		m_OutButtonEvent += eventFunc;
		break;
	case Button::IN_BUTTON:
		m_InButtonEvent += eventFunc;
		break;
	case Button::DOWN_BUTTON:
		m_DownButtonEvent += eventFunc;
		break;
	case Button::UP_BUTTON:
		m_UpButtonEvent += eventFunc;
		break;
	case Button::PRESS_DOWN_BUTTON:
		m_PressDownButtonEvent += eventFunc;
		break;
	default:
		break;
	}
}

void Button::PopEvent(std::function<void()>& eventFunc, State type)
{
	switch (type)
	{
	case Button::OUT_BUTTON:
		m_OutButtonEvent -= eventFunc;
		break;
	case Button::IN_BUTTON:
		m_InButtonEvent -= eventFunc;
		break;
	case Button::DOWN_BUTTON:
		m_DownButtonEvent -= eventFunc;
		break;
	case Button::UP_BUTTON:
		m_UpButtonEvent -= eventFunc;
		break;
	case Button::PRESS_DOWN_BUTTON:
		m_PressDownButtonEvent -= eventFunc;
		break;
	default:
		break;
	}
}

void Button::ResetEvent(State type)
{
	switch (type)
	{
	case Button::IN_BUTTON:
		m_InButtonEvent.Reset();
		break;
	case Button::OUT_BUTTON:
		m_OutButtonEvent.Reset();
		break;
	case Button::DOWN_BUTTON:
		m_DownButtonEvent.Reset();
		break;
	case Button::UP_BUTTON:
		m_UpButtonEvent.Reset();
		break;
	case Button::PRESS_DOWN_BUTTON:
		m_PressDownButtonEvent.Reset();
		break;
	default:
		break;
	}
}

void Button::ResetAllEvent()
{
	m_InButtonEvent.Reset();
	m_OutButtonEvent.Reset();
	m_DownButtonEvent.Reset();
	m_UpButtonEvent.Reset();
	m_PressDownButtonEvent.Reset();
}

