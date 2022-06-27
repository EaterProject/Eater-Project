#include "Button.h"
#include "GameObject.h"
#include "RectTransform.h"
#include "Image.h"
#include "KeyinputManager.h"

#include "Profiler/Profiler.h"

Button::Button()
	:m_State(OUT_BUTTON), m_Start(false)
{

}

Button::~Button()
{

}

void Button::Awake()
{
	m_Image = gameobject->GetComponent<Image>();
	m_Transform = gameobject->GetComponent<RectTransform>();
	m_Rect = m_Transform->GetRectPoint();

	// 만약 Image Component가 추가되지 않았다면..
	if (m_Image == nullptr)
	{
		m_Image = gameobject->AddComponent<Image>();
	}

	// 후 순위 설정 함수 리스트 실행..
	StartFunction();

	// 함수 실행후 리스트 초기화..
	StartFunction.Reset();

	// 최초 설정이 끝난후 상태 변경..
	m_Start = true;
}

void Button::Update()
{
	const LPPOINT& mouse_point = mKeyInputManger->GetClientMousePos();

	//PROFILE_LOG(PROFILE_OUTPUT::VS_CODE, "Mouse Point : %d, %d", mouse_point->x, mouse_point->y);

	if (m_Rect->left + m_Left_Offset < mouse_point->x && m_Rect->right + m_Right_Offset > mouse_point->x)
	{
		if (m_Rect->top + m_Top_Offset < mouse_point->y && m_Rect->bottom + m_Bottom_Offset > mouse_point->y)
		{
			// 마우스가 버튼 위에 있는 경우..
			switch (m_State)
			{
			case Button::OUT_BUTTON:
			{
				m_State = IN_BUTTON;

				// 등록된 이벤트 실행..
				m_InButtonEvent();
			}
				break;
			default:
				break;
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

void Button::PushEvent(std::function<void()>&& eventFunc, State type)
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

void Button::PopEvent(std::function<void()>&& eventFunc, State type)
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

void Button::SetBoundaryOffset(float left, float right, float top, float bottom)
{
	m_Left_Offset = left;
	m_Right_Offset = right;
	m_Top_Offset = top;
	m_Bottom_Offset = bottom;
}

void Button::SetPivot(PIVOT_TYPE pivot_type)
{
	if (m_Start)
	{
		m_Transform->SetPivot(pivot_type);
	}
	else
	{
		m_Pivot = pivot_type;
		StartFunction += Eater::Bind(static_cast<void(Button::*)(void)>(&Button::SetPivot), this);
	}
}

void Button::SetPivot()
{
	m_Transform->SetPivot(m_Pivot);
}

void Button::SetPosition(float x, float y)
{
	if (m_Start)
	{
		m_Transform->SetPosition(x, y);
	}
	else
	{
		m_Pos.x = x;
		m_Pos.y = y;
		StartFunction += Eater::Bind(static_cast<void(Button::*)(void)>(&Button::SetPivot), this);
	}
}

void Button::SetPosition()
{
	m_Transform->SetPosition(m_Pos);
}

void Button::SetRotation(float x, float y, float z)
{
	if (m_Start)
	{
		m_Transform->SetRotation(x, y, z);
	}
	else
	{
		m_Rot.x = x;
		m_Rot.y = y;
		m_Rot.z = z;
		StartFunction += Eater::Bind(static_cast<void(Button::*)(void)>(&Button::SetRotation), this);
	}
}

void Button::SetRotation()
{
	m_Transform->SetRotation(m_Rot);
}

void Button::SetScale(float x, float y)
{
	if (m_Start)
	{
		m_Transform->SetScale(x, y);
	}
	else
	{
		m_Scl.x = x;
		m_Scl.y = y;
		StartFunction += Eater::Bind(static_cast<void(Button::*)(void)>(&Button::SetScale), this);
	}
}

void Button::SetScale()
{
	m_Transform->SetScale(m_Scl);
}

void Button::AddPosition(float x, float y)
{
	if (m_Start)
	{
		m_Transform->AddPosition(x, y);
	}
	else
	{
		m_Pos.x += x;
		m_Pos.y += y;
		StartFunction += Eater::Bind(static_cast<void(Button::*)(void)>(&Button::SetPosition), this);
	}
}

void Button::AddRotation(float x, float y, float z)
{
	if (m_Start)
	{
		m_Transform->AddRotation(x, y, z);
	}
	else
	{
		m_Rot.x += x;
		m_Rot.y += y;
		m_Rot.z += z;
		StartFunction += Eater::Bind(static_cast<void(Button::*)(void)>(&Button::SetRotation), this);
	}
}

void Button::AddScale(float x, float y)
{
	if (m_Start)
	{
		m_Transform->AddScale(x, y);
	}
	else
	{
		m_Scl.x += x;
		m_Scl.y += y;
		StartFunction += Eater::Bind(static_cast<void(Button::*)(void)>(&Button::SetScale), this);
	}
}

void Button::SetTexture(std::string texture_name)
{
	if (m_Start)
	{
		m_Image->SetTexture(texture_name);
	}
	else
	{
		m_TextureName = texture_name;
		StartFunction += Eater::Bind(static_cast<void(Button::*)(void)>(&Button::SetTexture), this);
	}
}

void Button::SetTexture()
{
	m_Image->SetTexture(m_TextureName);
}

void Button::PushTextureList(std::string texture_name)
{
	if (m_Start)
	{
		m_Image->PushTextureList(texture_name);
	}
	else
	{
		m_TextureListName.push_back(texture_name);
		StartFunction += Eater::Bind(static_cast<void(Button::*)(void)>(&Button::PushTextureList), this);
	}
}

void Button::PushTextureList()
{
	for (auto& texture_name : m_TextureListName)
	{
		m_Image->PushTextureList(texture_name);
	}

	m_TextureListName.clear();
}

