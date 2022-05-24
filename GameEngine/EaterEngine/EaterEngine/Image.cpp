#include "Image.h"

#include "GameObject.h"
#include "RectTransform.h"
#include "UIManager.h"
#include "LoadManager.h"
#include "GraphicEngineAPI.h"


Image::Image()
{
	m_UI = new UIBuffer();
	m_UI->UI_Property = new UIProperty();

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
}

void Image::Start()
{
	//클라이언트쪽에서 텍스쳐의 이름을 넣고 모두 끝난상태
	if (isLoad_Texture)
	{
		SetTexture();
	}

	//실행도중 텍스쳐를 바꿀수있게 모두 true
	isLoad_Texture = true;
}

void Image::SetTexture(std::string texture_name)
{
	if (isLoad_Texture == false)
	{
		isLoad_Texture = true;
		TextureName = texture_name;
	}
	else
	{
		TextureName = texture_name;
		SetTexture();
	}
}

void Image::SetTexture()
{
	TextureBuffer* newTexture = LoadManager::GetTexture(TextureName);

	// Texture 변경..
	m_UI->Albedo = newTexture;

	// 그래픽 연동..


	// 이미지 크기 재설정..
	m_Transform->SetImageSize(Vector2(m_UI->Albedo->Width, m_UI->Albedo->Height));
}

void Image::SetImageColor(DirectX::SimpleMath::Vector4 image_color)
{
	m_UI->UI_Property->ImageColor = image_color;
}
