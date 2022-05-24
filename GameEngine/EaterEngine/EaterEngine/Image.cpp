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
	//Ŭ���̾�Ʈ�ʿ��� �ؽ����� �̸��� �ְ� ��� ��������
	if (isLoad_Texture)
	{
		SetTexture();
	}

	//���൵�� �ؽ��ĸ� �ٲܼ��ְ� ��� true
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

	// Texture ����..
	m_UI->Albedo = newTexture;

	// �׷��� ����..


	// �̹��� ũ�� �缳��..
	m_Transform->SetImageSize(Vector2(m_UI->Albedo->Width, m_UI->Albedo->Height));
}

void Image::SetImageColor(DirectX::SimpleMath::Vector4 image_color)
{
	m_UI->UI_Property->ImageColor = image_color;
}
