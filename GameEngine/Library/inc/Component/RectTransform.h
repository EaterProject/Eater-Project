#pragma once
#include "EaterEngineDLL.h"
#include "SimpleMath.h"
#include "Component.h"
#include "TypeOptionHeader.h"

struct RectPoint
{
	float left;
	float top;
	float right;
	float bottom;
};

class Transform;

class RectTransform : public Component
{
public:
	EATER_ENGINEDLL RectTransform();
	~RectTransform();

public:
	void TransformUpdate() override;

public:
	EATER_ENGINEDLL void SetImageSize(float x, float y);
	EATER_ENGINEDLL void SetImageSize(DirectX::SimpleMath::Vector2 image_size);

	EATER_ENGINEDLL void SetPivot(PIVOT_TYPE pivot_type);
	EATER_ENGINEDLL void SetPosition(float x, float y);
	EATER_ENGINEDLL void SetPosition(DirectX::SimpleMath::Vector2 pos);

	EATER_ENGINEDLL void SetPositionObject(GameObject* object, DirectX::SimpleMath::Vector3 offset);
	EATER_ENGINEDLL void SetPositionObject(Transform* object, DirectX::SimpleMath::Vector3 offset);
	EATER_ENGINEDLL void SetPosition3D(float x, float y, float z);
	EATER_ENGINEDLL void SetPosition3D(DirectX::SimpleMath::Vector3 pos);

	EATER_ENGINEDLL void SetRotation(float angle);
	EATER_ENGINEDLL void SetRotation(float x, float y, float z);
	EATER_ENGINEDLL void SetRotation(DirectX::SimpleMath::Vector3 rot);

	EATER_ENGINEDLL void SetScale(float x, float y);
	EATER_ENGINEDLL void SetScale(DirectX::SimpleMath::Vector2 scale);

	EATER_ENGINEDLL void AddPosition(float x, float y);
	EATER_ENGINEDLL void AddPosition(DirectX::SimpleMath::Vector2 pos);

	EATER_ENGINEDLL void AddRotation(float x, float y, float z);
	EATER_ENGINEDLL void AddRotation(DirectX::SimpleMath::Vector3 rot);

	EATER_ENGINEDLL void AddScale(float x, float y);
	EATER_ENGINEDLL void AddScale(DirectX::SimpleMath::Vector2 scale);

public:
	EATER_ENGINEDLL const RectPoint& GetRectPoint();

private:
	void Resize(int width, int height);

public:
	//�̹��� ��� ��ġ
	PIVOT_TYPE PivotType;
	//�̹��� ũ��
	DirectX::SimpleMath::Vector2 ImageSize;
	//�̹��� ����
	DirectX::SimpleMath::Vector4 ImageColor;

	//��ġ
	DirectX::SimpleMath::Vector2 Position;
	//ȸ��
	DirectX::SimpleMath::Vector3 Rotation;
	//ũ��
	DirectX::SimpleMath::Vector2 Scale;

private:
	DirectX::SimpleMath::Vector2 Screen_Resize;
	DirectX::SimpleMath::Vector2 Screen_Origin;
	DirectX::SimpleMath::Vector2 Screen_Ratio;

	DirectX::SimpleMath::Vector2 Position_Offset;
	DirectX::SimpleMath::Vector2 Scale_Offset;

	DirectX::SimpleMath::Matrix PositionXM;
	DirectX::SimpleMath::Matrix RotationXM;
	DirectX::SimpleMath::Matrix ScaleXM;

	DirectX::SimpleMath::Matrix WorldXM;

	//ȭ����� ��� ����
	RectPoint RectPosition;

	//3D Object
	Transform* Transform3D;
	DirectX::SimpleMath::Vector3 Position3D_Offset;
};

