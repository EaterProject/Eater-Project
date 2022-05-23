#include "EngineData.h"
#include "RectTransform.h"

#include "GameObject.h"
#include "GameEngine.h"
#include "LoadManager.h"
#include "GraphicEngineAPI.h"

#include "IndexManager.h"

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
}

RectTransform::~RectTransform()
{
	gameobject->OneMeshData->UI_Buffer = nullptr;

	delete m_UI->UI_Property;
	delete m_UI;
}

void RectTransform::Start()
{
	m_UI = new UIBuffer();
	m_UI->UI_Property = new UIProperty();

	IndexManager<UIBuffer>::PushResource(m_UI, &m_UI->BufferIndex);

	gameobject->OneMeshData->Object_Data->ObjType = OBJECT_TYPE::UI;

	gameobject->OneMeshData->UI_Buffer = m_UI;
}

void RectTransform::TransformUpdate()
{
	switch (PivotType)
	{
	case PIVOT_LEFT_TOP:
		break;
	case PIVOT_LEFT_BOTTOM:
		Position_Offset.y = GameEngine::WinSizeHeight - (ImageSize.y * Scale.y);
		break;
	case PIVOT_RIGHT_TOP:
		Position_Offset.x = GameEngine::WinSizeWidth - (ImageSize.x * Scale.x);
		break;
	case PIVOT_RIGHT_BOTTOM:
		Position_Offset.x = GameEngine::WinSizeWidth - (ImageSize.x * Scale.x);
		Position_Offset.y = GameEngine::WinSizeHeight - (ImageSize.y * Scale.y);
		break;
	case PIVOT_MIDDLE_TOP:
		Position_Offset.x = (GameEngine::WinSizeWidth - (ImageSize.x * Scale.x)) / 2.0f;
		break;
	case PIVOT_MIDDLE_BOTTOM:
		Position_Offset.x = (GameEngine::WinSizeWidth - (ImageSize.x * Scale.x)) / 2.0f;
		Position_Offset.y = GameEngine::WinSizeHeight - (ImageSize.y * Scale.y);
		break;
	case PIVOT_MIDDLE_LEFT:
		Position_Offset.y = (GameEngine::WinSizeHeight - (ImageSize.y * Scale.y)) / 2.0f;
		break;
	case PIVOT_MIDDLE_RIGHT:
		Position_Offset.x = GameEngine::WinSizeWidth - (ImageSize.x * Scale.x);
		Position_Offset.y = (GameEngine::WinSizeHeight - (ImageSize.y * Scale.y)) / 2.0f;
		break;
	case PIVOT_MIDDLE_CENTER:
		Position_Offset.x = (GameEngine::WinSizeWidth - (ImageSize.x * Scale.x)) / 2.0f;
		Position_Offset.y = (GameEngine::WinSizeHeight - (ImageSize.y * Scale.y)) / 2.0f;
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
	DirectX::XMMATRIX _P = DirectX::XMMatrixRotationX(radX);
	DirectX::XMMATRIX _Y = DirectX::XMMatrixRotationY(radY);
	DirectX::XMMATRIX _R = DirectX::XMMatrixRotationZ(radZ);

	RotationXM = DirectX::XMMatrixRotationRollPitchYaw(radX, radY, radZ);

	// Scale Matrix..
	ScaleXM._11 = Scale.x;
	ScaleXM._22 = Scale.y;

	// 최종 Matrix 설정..
	WorldXM = ScaleXM * RotationXM * PositionXM;
	m_UI->UI_Property->World = WorldXM;
}

void RectTransform::SetImage(std::string texture_name)
{
	TextureBuffer* newTexture = LoadManager::GetTexture(texture_name);

	// Texture 변경..
	m_UI->Albedo = newTexture;

	//gameobject->OneMeshData
}

void RectTransform::SetImagePivot(RECT_PIVOT pivot_type)
{
	PivotType = pivot_type;
}

void RectTransform::SetImageSize(DirectX::SimpleMath::Vector2 imagesize)
{
	ImageSize = imagesize;
}

void RectTransform::SetImageColor(DirectX::SimpleMath::Vector4 imagecolor)
{
	ImageColor = imagecolor;

	m_UI->UI_Property->ImageColor = imagecolor;
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

void RectTransform::AddPosition(DirectX::SimpleMath::Vector2 pos)
{
	Position += pos;
}

void RectTransform::AddRotation(DirectX::SimpleMath::Vector3 rot)
{
	Rotation += rot;
}

void RectTransform::AddScale(DirectX::SimpleMath::Vector2 scale)
{
	Scale += scale;
}