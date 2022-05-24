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

void Image::SetImage(std::string texture_name)
{
	TextureBuffer* newTexture = LoadManager::GetTexture(texture_name);

	// Texture 변경..
	m_UI->Albedo = newTexture;

	// 그래픽 연동..

}

void Image::SetImageColor(DirectX::SimpleMath::Vector4 image_color)
{
	m_UI->UI_Property->ImageColor = image_color;
}
