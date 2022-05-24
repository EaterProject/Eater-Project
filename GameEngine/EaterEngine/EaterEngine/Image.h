#pragma once
#include "Component.h"

class UIBuffer;
class RectTransform;

class Image : public Component
{
public:
	Image();
	virtual ~Image();

public:
	virtual void Awake() override;
	virtual void Start() override;

public:
	EATER_ENGINEDLL void SetTexture(std::string texture_name);
	EATER_ENGINEDLL void SetImageColor(DirectX::SimpleMath::Vector4 image_color);

private:
	void SetTexture();

protected:
	UIBuffer* m_UI;
	RectTransform* m_Transform;

private:
	bool isLoad_Texture;		//�ؽ��� �ε忩��

	std::string TextureName;		//�ε��� �ؽ��� �̸�
};

