#include "RectTransform.h"
#include "EngineData.h"

#include "GlobalDataManager.h"
#include "GameObject.h"
#include "GameEngine.h"
#include "Utility.h"
#include "Camera.h"
#include "GameObject.h"
#include "Transform.h"

RectTransform::RectTransform()
{
	PivotType			= PIVOT_TYPE::PIVOT_MIDDLE_CENTER;
	ImageSize			= { 0.0f, 0.0f };
	Position			= { 0.0f, 0.0f };
	Rotation			= { 0.0f, 0.0f, 0.0f };
	Scale				= { 0.5f, 0.5f };

	Position_Offset		= { 0.0f, 0.0f };
	Scale_Offset		= { 1.0f, 1.0f };

	Position3D_Offset	= { 0.0f, 0.0f, 0.0f };

	RotateAngle			= ROTATE_ANGLE::ROTATE_0;
	RotateImage			= false;
	Rotate_Offset		= 0.0f;
	Target_Ratio		= { 1.0f, 1.0f };

	PositionXM			= DirectX::XMMatrixIdentity();
	RotationXM			= DirectX::XMMatrixIdentity();
	ScaleXM				= DirectX::XMMatrixIdentity();

	Screen_Size.x		= GameEngine::WinSizeWidth;
	Screen_Size.y		= GameEngine::WinSizeHeight;
	
	GameEngine::ResizeFunction += Eater::Bind(&RectTransform::Resize, this);
}

RectTransform::~RectTransform()
{
	GameEngine::ResizeFunction -= Eater::Bind(&RectTransform::Resize, this);
}

void RectTransform::TransformUpdate()
{
	// Position Offset 설정..
	SetPositionOffset();

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
	RectPosition.size	= { ImageSize.x * Scale.x * 2.0f, ImageSize.y * Scale.y * 2.0f };
	RectPosition.center = { PositionXM._41, PositionXM._42 };
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

void RectTransform::SetPivot(PIVOT_TYPE pivot_type)
{
	PivotType = pivot_type;
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

void RectTransform::SetPositionObject(GameObject* object, DirectX::SimpleMath::Vector3 offset)
{
	Transform3D = object->GetTransform();
	Position3D_Offset = offset;
}

void RectTransform::SetPositionObject(Transform* object, DirectX::SimpleMath::Vector3 offset)
{
	Transform3D = object;
	Position3D_Offset = offset;
}

void RectTransform::SetPosition3D(float x, float y, float z)
{
	Vector2 screen_pos = Camera::g_MainCam->WorldToScreen(x, y, z);

	Position.x = screen_pos.x * Screen_Size.x;
	Position.y = screen_pos.y * Screen_Size.y;
}

void RectTransform::SetPosition3D(DirectX::SimpleMath::Vector3 pos)
{
	Vector2 screen_pos = Camera::g_MainCam->WorldToScreen(pos);

	Position.x = screen_pos.x * Screen_Size.x;
	Position.y = screen_pos.y * Screen_Size.y;
}

void RectTransform::SetTargetImage(GameObject* object, ROTATE_ANGLE rotate_angle)
{
	TargetRectPosition = object->GetComponent<RectTransform>()->GetRectPoint();
	RotateAngle = rotate_angle;

	switch (rotate_angle)
	{
	case ROTATE_0:
		Rotate_Offset = 0.0f;
		break;
	case ROTATE_90:
		Rotate_Offset = 90.0f;
		break;
	case ROTATE_180:
		Rotate_Offset = 180.0f;
		break;
	case ROTATE_270:
		Rotate_Offset = 270.0f;
		break;
	default:
		break;
	}
}

void RectTransform::SetTargetImage(RectTransform* object, ROTATE_ANGLE rotate_angle)
{
	TargetRectPosition = object->GetRectPoint();
	RotateAngle = rotate_angle;

	switch (rotate_angle)
	{
	case ROTATE_0:
		Rotate_Offset = 0.0f;
		break;
	case ROTATE_90:
		Rotate_Offset = 90.0f;
		break;
	case ROTATE_180:
		Rotate_Offset = 180.0f;
		break;
	case ROTATE_270:
		Rotate_Offset = 270.0f;
		break;
	default:
		break;
	}
}

void RectTransform::SetTargetRotation(bool enable)
{
	RotateImage = enable;
}

void RectTransform::SetTargetRatio(DirectX::SimpleMath::Vector2 ratio)
{
	Target_Ratio = ratio;
}

void RectTransform::SetTargetRatio(float x, float y)
{
	Target_Ratio.x = x;
	Target_Ratio.y = y;
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
	Scale.x = x * 0.5f;
	Scale.y = y * 0.5f;
}

void RectTransform::SetScale(DirectX::SimpleMath::Vector2 scale)
{
	Scale.x = scale.x * 0.5f;
	Scale.y = scale.y * 0.5f;
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

RectPoint* RectTransform::GetRectPoint()
{
	return &RectPosition;
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

void RectTransform::SetPositionOffset()
{
	switch (PivotType)
	{
	case PIVOT_TYPE::PIVOT_LEFT_TOP:
		Position_Offset.x = ImageSize.x * Scale.x;
		Position_Offset.y = ImageSize.y * Scale.y;
		break;
	case PIVOT_TYPE::PIVOT_LEFT_BOTTOM:
		Position_Offset.x = ImageSize.x * Scale.x;
		Position_Offset.y = Screen_Size.y - (ImageSize.y * Scale.y);
		break;
	case PIVOT_TYPE::PIVOT_RIGHT_TOP:
		Position_Offset.x = Screen_Size.x - (ImageSize.x * Scale.x);
		Position_Offset.y = ImageSize.y * Scale.y;
		break;
	case PIVOT_TYPE::PIVOT_RIGHT_BOTTOM:
		Position_Offset.x = Screen_Size.x - (ImageSize.x * Scale.x);
		Position_Offset.y = Screen_Size.y - (ImageSize.y * Scale.y);
		break;
	case PIVOT_TYPE::PIVOT_MIDDLE_TOP:
		Position_Offset.x = Screen_Size.x * 0.5f;
		Position_Offset.y = ImageSize.y * Scale.y;
		break;
	case PIVOT_TYPE::PIVOT_MIDDLE_BOTTOM:
		Position_Offset.x = Screen_Size.x * 0.5f;
		Position_Offset.y = Screen_Size.y - (ImageSize.y * Scale.y);
		break;
	case PIVOT_TYPE::PIVOT_MIDDLE_LEFT:
		Position_Offset.x = ImageSize.x * Scale.x;
		Position_Offset.y = Screen_Size.y * 0.5f;
		break;
	case PIVOT_TYPE::PIVOT_MIDDLE_RIGHT:
		Position_Offset.x = Screen_Size.x - (ImageSize.x * Scale.x);
		Position_Offset.y = Screen_Size.y * 0.5f;
		break;
	case PIVOT_TYPE::PIVOT_MIDDLE_CENTER:
		Position_Offset.x = Screen_Size.x * 0.5f;
		Position_Offset.y = Screen_Size.y * 0.5f;
		break;
	case PIVOT_TYPE::PIVOT_OBJECT:
		Position_Offset = Screen_Size * Camera::g_MainCam->WorldToScreen(Transform3D->GetPosition() + Position3D_Offset);
		break;
	case PIVOT_TYPE::PIVOT_IMAGE:
	{
		Vector3&& Position3D = Transform3D->GetPosition();

		switch (RotateAngle)
		{
		case ROTATE_0:
			Position2D.x = Position3D.x;
			Position2D.y = -Position3D.z;
			break;
		case ROTATE_90:
			Position2D.x = -Position3D.z;
			Position2D.y = -Position3D.x;
			break;
		case ROTATE_180:
			Position2D.x = -Position3D.x;
			Position2D.y = Position3D.z;
			break;
		case ROTATE_270:
			Position2D.x = Position3D.z;
			Position2D.y = Position3D.x;
			break;
		default:
			break;
		}

		if (RotateImage)
		{
			Rotation.z = -Transform3D->GetRotation().y + Rotate_Offset;
		}

		Position_Offset = TargetRectPosition->center + (TargetRectPosition->size * (Position2D / Target_Ratio));
	}
	break;
	default:
		break;
	}
}
