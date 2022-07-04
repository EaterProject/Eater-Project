#include "Image.h"

#include "GameObject.h"
#include "RectTransform.h"
#include "UIManager.h"
#include "LoadManager.h"
#include "GraphicEngineAPI.h"

Image::Image()
	:TextureIndex(-1), m_Start(false)
{
	m_UI = new UIBuffer();
	m_UI->UI_Property = new UIProperty();
	m_UI->UI_Property->ImageColor = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_UI->UI_Property->TexScale = Vector2(1.0f, 1.0f);
	m_UI->UI_Property->TexFill = Vector2(1.0f, 1.0f);
	m_UI->UI_Property->IsDraw = true;

	UIManager::SetIndex(&m_UI->BufferIndex);
}

Image::~Image()
{
	UIManager::DeleteIndex(m_UI->BufferIndex);

	gameobject->OneMeshData->UI_Buffer = nullptr;

	delete m_UI->UI_Property;
	delete m_UI;
}

void Image::Awake()
{
	m_Transform = gameobject->GetComponent<RectTransform>();

	gameobject->OneMeshData->Object_Data->ObjType = OBJECT_TYPE::UI;

	gameobject->OneMeshData->UI_Buffer = m_UI;

	// 후 순위 설정 함수 리스트 실행..
	StartFunction();

	// 함수 실행후 리스트 초기화..
	StartFunction.Reset();

	// 최초 설정이 끝난후 상태 변경..
	m_Start = true;
}

void Image::SetTexture(std::string texture_name)
{
	TextureName = texture_name;

	if (m_Start == false)
	{
		StartFunction += Eater::Bind(static_cast<void(Image::*)(void)>(&Image::SetTexture), this);
	}
	else
	{
		SetTexture();
	}
}

void Image::SetTexture()
{
	// 해당 이름의 해쉬코드..
	size_t hash_number = std::hash<std::string>()(TextureName);

	// 현재 인덱스와 동일할 경우..
	if (hash_number == TextureIndex) return;

	auto itor = TextureList.find(hash_number);

	TextureBuffer* nowTexture = nullptr;

	// 해당 인덱스의 텍스쳐가 없는 경우..
	if (itor == TextureList.end())
	{
		nowTexture = LoadManager::GetTexture(TextureName);

		// 해당 텍스쳐가 없는 경우..
		if (nowTexture == nullptr) return;

		TextureList.insert(std::pair<size_t, TextureBuffer*>(hash_number, nowTexture));
	}
	else
	{
		nowTexture = itor->second;
	}

	// 현재 텍스쳐 인덱스 변경..
	TextureIndex = hash_number;

	// Texture 변경..
	m_UI->Albedo = nowTexture;

	m_UI->UI_Property->UI_Name = nowTexture->Name;

	// 그래픽 연동..
	GraphicEngine::Get()->PushChangeInstance(gameobject->OneMeshData);

	// 이미지 크기 재설정..
	m_Transform->SetImageSize(m_UI->Albedo->Width, m_UI->Albedo->Height);
}

void Image::PushTextureList(std::string texture_name)
{
	size_t hash_number = std::hash<std::string>()(texture_name);

	auto itor = TextureList.find(hash_number);

	if (itor == TextureList.end())
	{
		TextureBuffer* newTexture = LoadManager::GetTexture(texture_name);

		TextureList.insert(std::pair<size_t, TextureBuffer*>(hash_number, newTexture));
	}
	else
	{

	}
}

void Image::PopTextureList(std::string texture_name)
{
	size_t hash_number = std::hash<std::string>()(texture_name);
	
	auto itor = TextureList.find(hash_number);

	if (itor != TextureList.end())
	{
		TextureList.erase(hash_number);
	}
	else
	{

	}
}

void Image::SetDraw(bool enable)
{
	m_UI->UI_Property->IsDraw = enable;
}

void Image::SetColor(DirectX::SimpleMath::Vector3 image_color)
{
	m_UI->UI_Property->ImageColor = Vector4(image_color.x / 255.0f, image_color.y / 255.0f, image_color.z / 255.0f, 1.0f);
}

void Image::SetColor(DirectX::SimpleMath::Vector4 image_color)
{
	m_UI->UI_Property->ImageColor = image_color / 255.0f;
}

void Image::SetColor(float r, float g, float b)
{
	m_UI->UI_Property->ImageColor.x = r / 255.0f;
	m_UI->UI_Property->ImageColor.y = g / 255.0f;
	m_UI->UI_Property->ImageColor.z = b / 255.0f;
}

void Image::SetColor(float r, float g, float b, float a)
{
	m_UI->UI_Property->ImageColor.x = r / 255.0f;
	m_UI->UI_Property->ImageColor.y = g / 255.0f;
	m_UI->UI_Property->ImageColor.z = b / 255.0f;
	m_UI->UI_Property->ImageColor.w = a / 255.0f;
}

void Image::SetAlpha(float alpha)
{
	m_UI->UI_Property->ImageColor.w = alpha / 255.0f;
}

void Image::SetActive(bool enable)
{
	this->gameobject->SetActive(enable);
}

void Image::SetPivot(PIVOT_TYPE pivot_type)
{
	m_Pivot = pivot_type;

	if (m_Start == false)
	{
		StartFunction += Eater::Bind(static_cast<void(Image::*)(void)>(&Image::SetPivot), this);
	}
	else
	{
		SetPivot();
	}
}

void Image::SetPivot()
{
	m_Transform->SetPivot(m_Pivot);
}

void Image::SetPosition(float x, float y)
{
	m_Position = { x,y };

	if (m_Start == false)
	{
		StartFunction += Eater::Bind(static_cast<void(Image::*)(void)>(&Image::SetPosition), this);
	}
	else
	{
		SetPosition();
	}
}

void Image::SetPosition(DirectX::SimpleMath::Vector2 pos)
{
	m_Position = pos;

	if (m_Start == false)
	{
		StartFunction += Eater::Bind(static_cast<void(Image::*)(void)>(&Image::SetPosition), this);
	}
	else
	{
		SetPosition();
	}
}

void Image::SetPosition()
{
	m_Transform->SetPosition(m_Position);
}

void Image::SetRotation(float x, float y, float z)
{
	m_Rotation = { x,y,z };

	if (m_Start == false)
	{
		StartFunction += Eater::Bind(static_cast<void(Image::*)(void)>(&Image::SetRotation), this);
	}
	else
	{
		SetRotation();
	}
}

void Image::SetRotation(DirectX::SimpleMath::Vector3 rot)
{
	m_Rotation = rot;

	if (m_Start == false)
	{
		StartFunction += Eater::Bind(static_cast<void(Image::*)(void)>(&Image::SetRotation), this);
	}
	else
	{
		SetRotation();
	}
}

void Image::SetRotation()
{
	m_Transform->SetRotation(m_Rotation);
}

void Image::SetScale(float x, float y)
{
	m_Scale = { x,y };

	if (m_Start == false)
	{
		StartFunction += Eater::Bind(static_cast<void(Image::*)(void)>(&Image::SetScale), this);
	}
	else
	{
		SetScale();
	}
}

void Image::SetScale(DirectX::SimpleMath::Vector2 scale)
{
	m_Scale = scale;

	if (m_Start == false)
	{
		StartFunction += Eater::Bind(static_cast<void(Image::*)(void)>(&Image::SetScale), this);
	}
	else
	{
		SetScale();
	}
}


void Image::SetScale()
{
	m_Transform->SetScale(m_Scale);
}

void Image::SetFillRange(FILL_TYPE type, float range)
{
	if (range >= 1.0f)
	{
		m_UI->UI_Property->UI_Option = UI_TYPE::UI_DEFAULT;
	}
	else
	{
		switch (type)
		{
		case FILL_UP:
			m_UI->UI_Property->TexFill.y = range;
			m_UI->UI_Property->UI_Option = UI_TYPE::UI_FILL;
			break;
		case FILL_DOWN:
			m_UI->UI_Property->TexFill.x = 0.0f;
			m_UI->UI_Property->TexFill.y = 1.0f - range;
			m_UI->UI_Property->UI_Option = UI_TYPE::UI_FILL_REVERSE;
			break;
		case FILL_LEFT:
			m_UI->UI_Property->TexFill.x = range;
			m_UI->UI_Property->UI_Option = UI_TYPE::UI_FILL;
			break;
		case FILL_RIGHT:
			m_UI->UI_Property->TexFill.x = 1.0f - range;
			m_UI->UI_Property->TexFill.y = 0.0f;
			m_UI->UI_Property->UI_Option = UI_TYPE::UI_FILL_REVERSE;
			break;
		default:
			break;
		}
	}
}

void Image::SetLayer(UINT order)
{
	m_UI->ChangeLayer = order;

	// 그래픽 연동..
	GraphicEngine::Get()->PushChangeInstance(gameobject->OneMeshData);
}

DirectX::SimpleMath::Vector2 Image::GetImageSize()
{
	return DirectX::SimpleMath::Vector2(m_UI->Albedo->Width, m_UI->Albedo->Height);
}

DirectX::SimpleMath::Vector4 Image::GetImageColor()
{
	return m_UI->UI_Property->ImageColor;
}

UINT Image::GetLayer()
{
	return m_UI->BufferLayer;
}
