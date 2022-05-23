#pragma once
#include "EaterEngineDLL.h"
#include "SimpleMath.h"
#include "Component.h"

class UIData;

class RectTransform : public Component
{
public:
	RectTransform();
	~RectTransform();

public:
	void Start() override;
	void TransformUpdate() override;

public:
	EATER_ENGINEDLL void SetImagePivot(RECT_PIVOT pivot_type);
	EATER_ENGINEDLL void SetImageSize(DirectX::SimpleMath::Vector2 image_size);
	EATER_ENGINEDLL void SetImageColor(DirectX::SimpleMath::Vector4 image_color);
	EATER_ENGINEDLL void SetPosition(DirectX::SimpleMath::Vector2 pos);
	EATER_ENGINEDLL void SetRotation(DirectX::SimpleMath::Vector3 rot);
	EATER_ENGINEDLL void SetScale(DirectX::SimpleMath::Vector2 scale);

	EATER_ENGINEDLL void SetTexFill(DirectX::SimpleMath::Vector2 fill_amount);

public:
	//이미지 출력 위치
	RECT_PIVOT PivotType;
	//이미지 크기
	DirectX::SimpleMath::Vector2 ImageSize;
	//이미지 색상
	DirectX::SimpleMath::Vector4 ImageColor;

	//이미지 출력 범위
	DirectX::SimpleMath::Vector2 ImageFillAmount;

	//위치
	DirectX::SimpleMath::Vector2 Position;
	//회전
	DirectX::SimpleMath::Vector3 Rotation;
	//크기
	DirectX::SimpleMath::Vector2 Scale;

private:
	DirectX::SimpleMath::Vector2 Position_Offset;

	DirectX::SimpleMath::Matrix PositionXM;
	DirectX::SimpleMath::Matrix RotationXM;
	DirectX::SimpleMath::Matrix ScaleXM;

	DirectX::SimpleMath::Matrix WorldXM;

	UIData* m_UI;
};

