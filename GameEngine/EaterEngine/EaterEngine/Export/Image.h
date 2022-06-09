#pragma once
#include "Component.h"

class UIBuffer;
class RectTransform;

class Image : public Component
{
public:
	EATER_ENGINEDLL Image();
	virtual ~Image();

public:
	void Awake() override;
	void Start() override;

public:
	EATER_ENGINEDLL void SetTexture(std::string texture_name);
	EATER_ENGINEDLL void SetImageColor(DirectX::SimpleMath::Vector3 image_color);
	EATER_ENGINEDLL void SetImageColor(DirectX::SimpleMath::Vector4 image_color);
	EATER_ENGINEDLL void SetImageColor(float r, float g, float b);
	EATER_ENGINEDLL void SetImageColor(float r, float g, float b, float a);
	EATER_ENGINEDLL void SetLayer(UINT order);

public:
	EATER_ENGINEDLL DirectX::SimpleMath::Vector2 GetImageSize();
	EATER_ENGINEDLL DirectX::SimpleMath::Vector4 GetImageColor();

private:
	void SetTexture();

private:
	bool isLoad_Texture;		//텍스쳐 로드여부

	std::string TextureName;	//로드한 텍스쳐 이름

private:
	UIBuffer* m_UI;
	RectTransform* m_Transform;
};

