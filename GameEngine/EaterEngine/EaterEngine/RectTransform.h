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
	//�ǹ�
	DirectX::SimpleMath::Vector2 ImagePivot;
	//�̹��� ũ��
	DirectX::SimpleMath::Vector2 ImageSize;
	//�̹��� ����
	DirectX::SimpleMath::Vector4 ImageColor;

	//Tex
	DirectX::SimpleMath::Vector2 ImageFillAmount;

	//��ġ
	DirectX::SimpleMath::Vector2 Position;
	//ȸ��
	DirectX::SimpleMath::Vector3 Rotation;
	//ũ��
	DirectX::SimpleMath::Vector2 Scale;

private:
	UIData* m_UI;
};

