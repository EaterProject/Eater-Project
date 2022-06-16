#include "Slider.h"
#include "GameObject.h"
#include "Image.h"
#include "RectTransform.h"

Slider::Slider()
	:m_MinValue(0.0f), m_MaxValue(1.0f), m_Value(1.0f), m_Start(false)
{

}

Slider::~Slider()
{

}

void Slider::Awake()
{
	// Image Transform 가져오기..
	if (m_BackImage) m_BackTransform = m_BackImage->gameobject->GetComponent<RectTransform>();
	if (m_FillImage) m_FillTransform = m_FillImage->gameobject->GetComponent<RectTransform>();

	// 후 순위 설정 함수 리스트 실행..
	StartFunction();

	// 함수 실행후 리스트 초기화..
	StartFunction.Reset();

	// 항상 배경이 뒤로 오게 한다..
	if (m_BackImage && m_FillImage)
	{
		m_FillImage->SetLayer(m_BackImage->GetLayer() + 1);
	}

	// 최초 설정이 끝난후 상태 변경..
	m_Start = true;
}

void Slider::SetValueRange(float min, float max)
{
	if (min <= max)
	{
		m_MinValue = min;
		m_MaxValue = max;
	}
	else
	{
		m_MinValue = max;
		m_MaxValue = min;
	}

	// 변환 범위 설정..
	float value_percent = m_MaxValue - m_MinValue;

	if (value_percent == 0.0f)
	{
		m_Value = 1.0f;
	}
	else
	{
		m_Value = 1.0f / value_percent;
	}
}

void Slider::SetPivot(RECT_PIVOT pivot_type)
{
	m_Pivot = pivot_type;

	if (m_Start == false)
	{
		StartFunction += Eater::Bind(static_cast<void(Slider::*)(void)>(&Slider::SetPivot), this);
	}
	else
	{
		SetPivot();
	}
}

void Slider::SetPivot()
{
	if (m_BackTransform) m_BackTransform->SetPivot(m_Pivot);
	if (m_FillTransform) m_FillTransform->SetPivot(m_Pivot);
}

void Slider::SetPosition(DirectX::SimpleMath::Vector2 pos)
{
	m_Position = pos;

	if (m_Start == false)
	{
		StartFunction += Eater::Bind(static_cast<void(Slider::*)(void)>(&Slider::SetPosition), this);
	}
	else
	{
		SetPosition();
	}
}

void Slider::SetPosition(float x, float y)
{
	m_Position = {x, y};

	if (m_Start == false)
	{
		StartFunction += Eater::Bind(static_cast<void(Slider::*)(void)>(&Slider::SetPosition), this);
	}
	else
	{
		SetPosition();
	}
}

void Slider::SetPosition()
{
	if (m_BackTransform) m_BackTransform->SetPosition(m_Position);
	if (m_FillTransform) m_FillTransform->SetPosition(m_Position);
}

void Slider::SetBackGroundImage(Image* img)
{
	m_BackImage = img;
}

void Slider::SetFillImage(Image* img)
{
	m_FillImage = img;
}

void Slider::SetBackGroundTexture(std::string texture_name)
{
	m_BackTextureName = texture_name;

	if (m_Start == false)
	{
		StartFunction += Eater::Bind(static_cast<void(Slider::*)(void)>(&Slider::SetBackGroundTexture), this);
	}
	else
	{
		SetBackGroundTexture();
	}
}

void Slider::SetBackGroundTexture()
{
	if (m_BackImage) m_BackImage->SetTexture(m_BackTextureName);
}

void Slider::SetFillTexture(std::string texture_name)
{
	m_FillTextureName = texture_name;
	
	if (m_Start == false)
	{
		StartFunction += Eater::Bind(static_cast<void(Slider::*)(void)>(&Slider::SetFillTexture), this);
	}
	else
	{
		SetFillTexture();
	}
}

void Slider::SetFillTexture()
{
	if (m_FillImage) m_FillImage->SetTexture(m_FillTextureName);
}

void Slider::SetBackGroundColor(DirectX::SimpleMath::Vector4 image_color)
{
	m_BackColor = image_color;

	if (m_Start == false)
	{
		StartFunction += Eater::Bind(static_cast<void(Slider::*)(void)>(&Slider::SetBackGroundColor), this);
	}
	else
	{
		SetBackGroundColor();
	}
}

void Slider::SetBackGroundColor(float r, float g, float b, float a)
{
	m_BackColor = {r, g, b, a};

	if (m_Start == false)
	{
		StartFunction += Eater::Bind(static_cast<void(Slider::*)(void)>(&Slider::SetBackGroundColor), this);
	}
	else
	{
		SetBackGroundColor();
	}
}

void Slider::SetBackGroundColor()
{
	if (m_BackImage) m_BackImage->SetImageColor(m_BackColor);
}

void Slider::SetFillColor(DirectX::SimpleMath::Vector4 image_color)
{
	m_FillColor = image_color;

	if (m_Start == false)
	{
		StartFunction += Eater::Bind(static_cast<void(Slider::*)(void)>(&Slider::SetFillColor), this);
	}
	else
	{
		SetFillColor();
	}
}

void Slider::SetFillColor(float r, float g, float b, float a)
{
	m_FillColor = { r, g, b, a };

	if (m_Start == false)
	{
		StartFunction += Eater::Bind(static_cast<void(Slider::*)(void)>(&Slider::SetFillColor), this);
	}
	else
	{
		SetFillColor();
	}
}

void Slider::SetFillColor()
{
	if (m_FillImage) m_FillImage->SetImageColor(m_FillColor);
}

void Slider::SetFillRange(FILL_TYPE fill_type, float range)
{
	if (range < m_MinValue)
	{
		range = m_MinValue;
	}
	if (range > m_MaxValue)
	{
		range = m_MaxValue;
	}

	// 설정된 비율의 범위 설정..
	m_FillRange = range * m_Value;

	if (m_Start == false)
	{
		StartFunction += Eater::Bind(static_cast<void(Slider::*)(void)>(&Slider::SetFillRange), this);
	}
	else
	{
		SetFillRange();
	}
}

void Slider::SetFillRange()
{
	if (m_FillImage) m_FillImage->SetFillRange(m_FillType, m_FillRange);
}
