#include "Button.h"
#include "GameObject.h"
#include "RectTransform.h"
#include "Image.h"
#include "KeyinputManager.h"

#define DEFALT_IMAGE
#define ON_IMAGE
#define CLICK_IMAGE

Button::Button()
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
}

void Button::Update()
{
	const RectPoint& rect_point = m_Transform->GetRectPoint();
	const LPPOINT& mouse_point = mKeyInputManger->GetMousePos();

	if (rect_point.left < mouse_point->x && rect_point.right > mouse_point->x)
	{
		if (rect_point.top < mouse_point->y && rect_point.bottom > mouse_point->y)
		{
			// 마우스가 버튼 위에 있는 경우..

			// 마우스가 클릭 상태인 경우..
			if (mKeyInputManger->GetKeyDown(VK_LBUTTON))
			{
				// 등록된 이벤트 실행..
				m_OnClickEvent();
			}
		}
	}
}

void Button::PushOnClickEvent(std::function<void()>& eventFunc)
{
	m_OnClickEvent += std::forward<std::function<void()>>(eventFunc);
}

void Button::PopOnClickEvent(std::function<void()>& eventFunc)
{
	m_OnClickEvent -= std::forward<std::function<void()>>(eventFunc);
}

