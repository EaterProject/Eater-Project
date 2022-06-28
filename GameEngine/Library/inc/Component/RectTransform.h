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
	Vector2 size;
	Vector2 center;
};

class Transform;

class RectTransform : public Component
{
public:
	EATER_ENGINEDLL RectTransform();
	~RectTransform();

private:
	enum UPDATE_TYPE
	{
		NONE,
		ONECE,
		ALWAYS,
	};

public:
	void TransformUpdate() override;

public:
	EATER_ENGINEDLL void SetImageSize(float x, float y);
	EATER_ENGINEDLL void SetImageSize(DirectX::SimpleMath::Vector2 image_size);

	EATER_ENGINEDLL void SetPivot(PIVOT_TYPE pivot_type);
	EATER_ENGINEDLL void SetPosition(float x, float y);
	EATER_ENGINEDLL void SetPosition(DirectX::SimpleMath::Vector2 pos);

	// Object & Image Pivot 전용
	EATER_ENGINEDLL void SetPositionObject(GameObject* object, DirectX::SimpleMath::Vector3 offset = DirectX::SimpleMath::Vector3(0.0f));
	EATER_ENGINEDLL void SetPositionObject(Transform* object, DirectX::SimpleMath::Vector3 offset = DirectX::SimpleMath::Vector3(0.0f));
	EATER_ENGINEDLL void SetPosition3D(float x, float y, float z);
	EATER_ENGINEDLL void SetPosition3D(DirectX::SimpleMath::Vector3 pos);

	// Image Pivot 전용
	EATER_ENGINEDLL void SetTargetImage(GameObject* object, ROTATE_ANGLE rotate_angle = ROTATE_ANGLE::ROTATE_0);
	EATER_ENGINEDLL void SetTargetImage(RectTransform* object, ROTATE_ANGLE rotate_angle = ROTATE_ANGLE::ROTATE_0);
	EATER_ENGINEDLL void SetTargetRotation(bool enable);
	EATER_ENGINEDLL void SetTargetRatio(float x, float y);
	EATER_ENGINEDLL void SetTargetRatio(DirectX::SimpleMath::Vector2 ratio);

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
	EATER_ENGINEDLL RectPoint* GetRectPoint();

private:
	void Resize(int width, int height);

private:
	void UpdateTransform();
	void SetPositionOffset();

public:
	//이미지 출력 위치
	PIVOT_TYPE PivotType;
	//이미지 크기
	DirectX::SimpleMath::Vector2 ImageSize;
	//이미지 색상
	DirectX::SimpleMath::Vector4 ImageColor;

	//위치
	DirectX::SimpleMath::Vector2 Position;
	//회전
	DirectX::SimpleMath::Vector3 Rotation;
	//크기
	DirectX::SimpleMath::Vector2 Scale;

private:
	UPDATE_TYPE UpdateType;

	DirectX::SimpleMath::Vector2 Screen_Resize;
	DirectX::SimpleMath::Vector2 Screen_Origin;
	DirectX::SimpleMath::Vector2 Screen_Ratio;

	DirectX::SimpleMath::Vector2 Position_Offset;
	DirectX::SimpleMath::Vector2 Scale_Offset;

	DirectX::SimpleMath::Matrix PositionXM;
	DirectX::SimpleMath::Matrix RotationXM;
	DirectX::SimpleMath::Matrix ScaleXM;

	DirectX::SimpleMath::Matrix WorldXM;

	//화면상의 출력 범위
	RectPoint RectPosition;

	//3D Object Pivot
	Transform* Transform3D;
	DirectX::SimpleMath::Vector3 Position3D_Offset;

	// Image Pivot
	RectPoint* TargetRectPosition;
	ROTATE_ANGLE RotateAngle;

	bool RotateImage;
	float Rotate_Offset;

	DirectX::SimpleMath::Vector2 Target_Ratio;
	DirectX::SimpleMath::Vector2 Position2D;
};

