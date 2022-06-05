#include "RectTransform.h"
#include "EngineData.h"

#include "GameObject.h"
#include "GameEngine.h"
#include "Utility.h"

RectTransform::RectTransform()
{
	PivotType		= RECT_PIVOT::PIVOT_MIDDLE_CENTER;
	ImageSize		= { 0, 0 };
	Position		= { 0, 0 };
	Rotation		= { 0, 0, 0 };
	Scale			= { 1, 1 };

	Position_Offset = { 0, 0 };
	Scale_Offset	= { 1, 1 };

	PositionXM		= DirectX::XMMatrixIdentity();
	RotationXM		= DirectX::XMMatrixIdentity();
	ScaleXM			= DirectX::XMMatrixIdentity();

	Screen_Size.x = GameEngine::WinSizeWidth;
	Screen_Size.y = GameEngine::WinSizeHeight;

	GameEngine::ResizeFunction += std::bind_front(&RectTransform::Resize, this);
}

RectTransform::~RectTransform()
{
	GameEngine::ResizeFunction -= std::bind_front(&RectTransform::Resize, this);
}

void RectTransform::TransformUpdate()
{
	switch (PivotType)
	{
	case PIVOT_LEFT_TOP:
		Position_Offset.x = ImageSize.x * Scale.x;
		Position_Offset.y = ImageSize.y * Scale.y;
		break;
	case PIVOT_LEFT_BOTTOM:
		Position_Offset.x = ImageSize.x * Scale.x;
		Position_Offset.y = GameEngine::WinSizeHeight - (ImageSize.y * Scale.y);
		break;
	case PIVOT_RIGHT_TOP:
		Position_Offset.x = GameEngine::WinSizeWidth - (ImageSize.x * Scale.x);
		Position_Offset.y = ImageSize.y * Scale.y;
		break;
	case PIVOT_RIGHT_BOTTOM:
		Position_Offset.x = GameEngine::WinSizeWidth - (ImageSize.x * Scale.x);
		Position_Offset.y = GameEngine::WinSizeHeight - (ImageSize.y * Scale.y);
		break;
	case PIVOT_MIDDLE_TOP:
		Position_Offset.x = GameEngine::WinSizeWidth * 0.5f;
		Position_Offset.y = ImageSize.y * Scale.y;
		break;
	case PIVOT_MIDDLE_BOTTOM:
		Position_Offset.x = GameEngine::WinSizeWidth * 0.5f;
		Position_Offset.y = GameEngine::WinSizeHeight - (ImageSize.y * Scale.y);
		break;
	case PIVOT_MIDDLE_LEFT:
		Position_Offset.x = ImageSize.x * Scale.x;
		Position_Offset.y = GameEngine::WinSizeHeight * 0.5f;
		break;
	case PIVOT_MIDDLE_RIGHT:
		Position_Offset.x = GameEngine::WinSizeWidth - (ImageSize.x * Scale.x);
		Position_Offset.y = GameEngine::WinSizeHeight * 0.5f;
		break;
	case PIVOT_MIDDLE_CENTER:
		Position_Offset.x = GameEngine::WinSizeWidth * 0.5f;
		Position_Offset.y = GameEngine::WinSizeHeight * 0.5f;
		break;
	default:
		break;
	}

	// Position Matrix..
	PositionXM._41 = Position_Offset.x + Position.x;
	PositionXM._42 = Position_Offset.y + Position.y;

	// Rotation Matrix..
	float radX = Rotation.x * 3.141592f / 180.0f;
	float radY = Rotation.y * 3.141592f / 180.0f;
	float radZ = Rotation.z * 3.141592f / 180.0f;

	RotationXM = DirectX::XMMatrixRotationRollPitchYaw(radX, radY + XM_PI, radZ + XM_PI);

	// Scale Matrix..
	ScaleXM._11 = ImageSize.x * Scale.x;
	ScaleXM._22 = ImageSize.y * Scale.y;

	// 최종 Matrix 설정..
	WorldXM = ScaleXM * RotationXM * PositionXM;
	gameobject->OneMeshData->UI_Buffer->UI_Property->World = WorldXM;

	// 최종 출력 범위 설정..
	float half_width = ImageSize.x * Scale.x * 0.5f;
	float half_height = ImageSize.y * Scale.y * 0.5f;

	RectPosition.left	= PositionXM._41 - half_width;
	RectPosition.top	= PositionXM._42 - half_height;
	RectPosition.right	= PositionXM._41 + half_width;
	RectPosition.bottom = PositionXM._42 + half_height;
}

void RectTransform::SetImagePivot(RECT_PIVOT pivot_type)
{
	PivotType = pivot_type;
}

void RectTransform::SetImageSize(float x, float y)
{
	ImageSize.x = x;
	ImageSize.y = y;
}

void RectTransform::SetImageSize(DirectX::SimpleMath::Vector2 imagesize)
{
	ImageSize = imagesize;
}

void RectTransform::SetPosition(float x, float y)
{
	Position.x = x;
	Position.y = y;
}

void RectTransform::SetPosition(DirectX::SimpleMath::Vector2 pos)
{
	Position = pos;
}

void RectTransform::SetRotation(float x, float y, float z)
{
	Rotation.x = x;
	Rotation.y = y;
	Rotation.z = z;
}

void RectTransform::SetRotation(DirectX::SimpleMath::Vector3 rot)
{
	Rotation = rot;
}

void RectTransform::SetRotation(float angle)
{
	Rotation.z = angle;
}

void RectTransform::SetScale(float x, float y)
{
	Scale.x = x;
	Scale.y = y;
}

void RectTransform::SetScale(DirectX::SimpleMath::Vector2 scale)
{
	Scale = scale;
}

void RectTransform::AddPosition(float x, float y)
{
	Position.x += x;
	Position.y += y;
}

void RectTransform::AddPosition(DirectX::SimpleMath::Vector2 pos)
{
	Position += pos;
}

void RectTransform::AddRotation(float x, float y, float z)
{
	Rotation.x += x;
	Rotation.y += y;
	Rotation.z += z;
}

void RectTransform::AddRotation(DirectX::SimpleMath::Vector3 rot)
{
	Rotation += rot;
}

void RectTransform::AddScale(float x, float y)
{
	Scale.x += x;
	Scale.y += y;
}

void RectTransform::AddScale(DirectX::SimpleMath::Vector2 scale)
{
	Scale += scale;
}

const RectPoint& RectTransform::GetRectPoint()
{
	return RectPosition;
}

void RectTransform::Resize(int width, int height)
{
	float ratio_w = (float)width / Screen_Size.x;
	float ratio_h = (float)height / Screen_Size.y;

	Scale.x *= ratio_w;
	Scale.y *= ratio_h;

	Position.x *= ratio_w;
	Position.y *= ratio_h;

	Screen_Size.x = width;
	Screen_Size.y = height;
}
