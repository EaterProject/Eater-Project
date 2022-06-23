#pragma once
#include "Component.h"
#include "Utility.h"

class Image;
class GameObject;
class Transform;
class RectTransform;

class Slider : public Component
{
public:
	EATER_ENGINEDLL Slider();
	virtual ~Slider();

public:
	void Awake() override;

public:
	EATER_ENGINEDLL void SetActive(bool enable);
	EATER_ENGINEDLL void SetValueRange(float min, float max);

	EATER_ENGINEDLL void SetPivot(PIVOT_TYPE pivot_type);

	EATER_ENGINEDLL void SetPosition(DirectX::SimpleMath::Vector2 pos);
	EATER_ENGINEDLL void SetPosition(float x, float y);

	EATER_ENGINEDLL void SetPositionObject(GameObject* object, DirectX::SimpleMath::Vector3 offset);
	EATER_ENGINEDLL void SetPositionObject(Transform* object, DirectX::SimpleMath::Vector3 offset);
	EATER_ENGINEDLL void SetPosition3D(float x, float y, float z);
	EATER_ENGINEDLL void SetPosition3D(DirectX::SimpleMath::Vector3 pos);

	EATER_ENGINEDLL void SetBackGroundImage(Image* img);
	EATER_ENGINEDLL void SetFillImage(Image* img);

	EATER_ENGINEDLL void SetBackGroundTexture(std::string texture_name);
	EATER_ENGINEDLL void SetFillTexture(std::string texture_name);

	EATER_ENGINEDLL void SetBackGroundColor(DirectX::SimpleMath::Vector4 image_color);
	EATER_ENGINEDLL void SetBackGroundColor(float r, float g, float b, float a);
	EATER_ENGINEDLL void SetFillColor(DirectX::SimpleMath::Vector4 image_color);
	EATER_ENGINEDLL void SetFillColor(float r, float g, float b, float a);
	EATER_ENGINEDLL void SetFillRange(FILL_TYPE fill_type, float range);

private:
	void SetPivot();
	void SetPosition();
	void SetPositionObject();
	void SetPosition3D();
	void SetBackGroundTexture();
	void SetFillTexture();
	void SetBackGroundColor();
	void SetFillColor();
	void SetFillRange();

private:
	Eater::Delegate<> StartFunction;

	bool m_Start;

	Transform* m_TargetObject;

	PIVOT_TYPE m_Pivot;
	Vector2 m_Position;
	Vector3 m_Position3D;
	Vector3 m_Position3D_Offset;

	Vector4 m_BackColor;
	Vector4 m_FillColor;

	FILL_TYPE m_FillType;
	float m_FillRange;

	std::string m_BackTextureName;
	std::string m_FillTextureName;

private:
	float m_Value;
	float m_MinValue;
	float m_MaxValue;

	Image* m_BackImage;
	Image* m_FillImage;

	RectTransform* m_BackTransform;
	RectTransform* m_FillTransform;
};
