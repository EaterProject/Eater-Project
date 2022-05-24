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

public:
	EATER_ENGINEDLL void SetImage(std::string texture_name);
	EATER_ENGINEDLL void SetImageColor(DirectX::SimpleMath::Vector4 image_color);

protected:
	UIBuffer* m_UI;
	RectTransform* m_Transform;
};

