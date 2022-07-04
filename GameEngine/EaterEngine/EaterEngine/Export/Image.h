#pragma once
#include <unordered_map>
#include "Component.h"
#include "EngineData.h"
#include "Utility.h"

class UIBuffer;
class RectTransform;

class Image : public Component
{
public:
	EATER_ENGINEDLL Image();
	virtual ~Image();

public:
	void Awake() override;

public:
	EATER_ENGINEDLL void SetTexture(std::string texture_name);
	EATER_ENGINEDLL void PushTextureList(std::string texture_name);
	EATER_ENGINEDLL void PopTextureList(std::string texture_name);

	EATER_ENGINEDLL void SetDraw(bool enable);
	EATER_ENGINEDLL void SetActive(bool enable);

	EATER_ENGINEDLL void SetPivot(PIVOT_TYPE pivot_type);

	EATER_ENGINEDLL void SetPosition(float x, float y);
	EATER_ENGINEDLL void SetPosition(DirectX::SimpleMath::Vector2 pos);

	EATER_ENGINEDLL void SetRotation(float x, float y, float z);
	EATER_ENGINEDLL void SetRotation(DirectX::SimpleMath::Vector3 rot);

	EATER_ENGINEDLL void SetScale(float x, float y);
	EATER_ENGINEDLL void SetScale(DirectX::SimpleMath::Vector2 scale);

	EATER_ENGINEDLL void SetColor(DirectX::SimpleMath::Vector3 image_color);
	EATER_ENGINEDLL void SetColor(DirectX::SimpleMath::Vector4 image_color);
	EATER_ENGINEDLL void SetColor(float r, float g, float b);
	EATER_ENGINEDLL void SetColor(float r, float g, float b, float a);
	EATER_ENGINEDLL void SetAlpha(float alpha);

	EATER_ENGINEDLL void SetFillRange(FILL_TYPE type, float range);
	EATER_ENGINEDLL void SetLayer(UINT order);

public:
	EATER_ENGINEDLL DirectX::SimpleMath::Vector2 GetImageSize();
	EATER_ENGINEDLL DirectX::SimpleMath::Vector4 GetImageColor();
	EATER_ENGINEDLL UINT GetLayer();

private:
	void SetTexture();
	void SetPivot();
	void SetPosition();
	void SetRotation();
	void SetScale();

private:
	Eater::Delegate<> StartFunction;
	
	bool m_Start;
	bool m_Draw;

	PIVOT_TYPE m_Pivot;
	Vector2 m_Position;
	Vector3 m_Rotation;
	Vector2 m_Scale;

	std::string TextureName;

private:
	size_t TextureIndex;

	std::unordered_map<size_t, TextureBuffer*> TextureList;

private:
	UIBuffer* m_UI;
	RectTransform* m_Transform;
};

