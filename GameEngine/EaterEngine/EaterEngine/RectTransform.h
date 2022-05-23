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
	EATER_ENGINEDLL void SetImagePivot(DirectX::SimpleMath::Vector2 imagepivot);
	EATER_ENGINEDLL void SetImageSize(DirectX::SimpleMath::Vector2 imagesize);
	EATER_ENGINEDLL void SetImageColor(DirectX::SimpleMath::Vector4 imagecolor);
	EATER_ENGINEDLL void SetPosition(DirectX::SimpleMath::Vector2 pos);
	EATER_ENGINEDLL void SetRotation(DirectX::SimpleMath::Vector3 rot);
	EATER_ENGINEDLL void SetScale(DirectX::SimpleMath::Vector2 scale);

	EATER_ENGINEDLL void SetTexFill(DirectX::SimpleMath::Vector2 fillamount);

public:
	//피벗
	DirectX::SimpleMath::Vector2 ImagePivot;
	//이미지 크기
	DirectX::SimpleMath::Vector2 ImageSize;
	//이미지 색상
	DirectX::SimpleMath::Vector4 ImageColor;

	//Tex
	DirectX::SimpleMath::Vector2 ImageFillAmount;

	//위치
	DirectX::SimpleMath::Vector2 Position;
	//회전
	DirectX::SimpleMath::Vector3 Rotation;
	//크기
	DirectX::SimpleMath::Vector2 Scale;

private:
	UIData* m_UI;
};

