#include "RectTransform.h"
#include "EngineData.h"
#include "GameObject.h"

RectTransform::RectTransform()
{
	ImagePivot	= { 0, 0 };
	ImageSize	= { 0, 0 };
	Position	= { 0, 0 };
	Rotation	= { 0, 0, 0 };
	Scale		= { 1, 1 };
	ImageTex	= { 0, 0};
}

RectTransform::~RectTransform()
{
	gameobject->OneMeshData->UI_Data = nullptr;

	delete m_UI;
}

void RectTransform::Start()
{
	m_UI = new UIData();

	gameobject->OneMeshData->UI_Data = m_UI;
}

void RectTransform::TransformUpdate()
{

}

void RectTransform::SetImagePivot(DirectX::SimpleMath::Vector2 imagepivot)
{
	ImagePivot = imagepivot;
}

void RectTransform::SetImageSize(DirectX::SimpleMath::Vector2 imagesize)
{
	ImageSize = imagesize;
}

void RectTransform::SetImageColor(DirectX::SimpleMath::Vector4 imagecolor)
{
	ImageColor = imagecolor;

	m_UI->ImageColor = imagecolor;
}

void RectTransform::SetPosition(DirectX::SimpleMath::Vector2 pos)
{
	Position = pos;
}

void RectTransform::SetRotation(DirectX::SimpleMath::Vector3 rot)
{
	Rotation = rot;
}

void RectTransform::SetScale(DirectX::SimpleMath::Vector2 scale)
{
	Scale = scale;
}

void RectTransform::SetTexPosition(DirectX::SimpleMath::Vector2 pos)
{

}
