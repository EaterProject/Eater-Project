#pragma once
#include "Component.h"
#include "Utility.h"

class Image;
class RectTransform;
struct RectPoint;

class Button : public Component
{
public:
	EATER_ENGINEDLL Button();
	virtual ~Button();

public:
	enum State
	{
		OUT_BUTTON,			// ��ư ������ ���콺�� ���� ���
		IN_BUTTON,			// ��ư ������ ���콺�� �ִ� ���
		DOWN_BUTTON,		// ��ư�� ������ ���
		UP_BUTTON,			// ��ư�� ���� ���
		PRESS_DOWN_BUTTON,	// ��ư�� ������ ���� ���
	};

public:
	void Awake() override;
	void Update() override;

public:
	EATER_ENGINEDLL void SetActive(bool active);
	
	EATER_ENGINEDLL void PushEvent(std::function<void()>& eventFunc, State type);
	EATER_ENGINEDLL void PushEvent(std::function<void()>&& eventFunc, State type);
	EATER_ENGINEDLL void PopEvent(std::function<void()>& eventFunc, State type);
	EATER_ENGINEDLL void PopEvent(std::function<void()>&& eventFunc, State type);

	EATER_ENGINEDLL void ResetEvent(State type);
	EATER_ENGINEDLL void ResetAllEvent();

	EATER_ENGINEDLL void SetBoundaryOffset(float left, float right, float top, float bottom);

	EATER_ENGINEDLL void SetPivot(PIVOT_TYPE pivot_type);
	EATER_ENGINEDLL void SetPosition(float x, float y);
	EATER_ENGINEDLL void SetRotation(float x, float y, float z);
	EATER_ENGINEDLL void SetScale(float x, float y);

	EATER_ENGINEDLL void AddPosition(float x, float y);
	EATER_ENGINEDLL void AddRotation(float x, float y, float z);
	EATER_ENGINEDLL void AddScale(float x, float y);

	EATER_ENGINEDLL void SetTexture(std::string texture_name);
	EATER_ENGINEDLL void PushTextureList(std::string texture_name);
	
	EATER_ENGINEDLL void SetLayer(UINT order);

private:
	void Resize(int width, int height);

private:
	void SetPivot();
	void SetPosition();
	void SetRotation();
	void SetScale();
	void SetTexture();
	void PushTextureList();
	void SetLayer();

private:
	Eater::Delegate<> m_OutButtonEvent;
	Eater::Delegate<> m_InButtonEvent;
	Eater::Delegate<> m_DownButtonEvent;
	Eater::Delegate<> m_UpButtonEvent;
	Eater::Delegate<> m_PressDownButtonEvent;

	State m_State;

	Image* m_Image;
	RectTransform* m_Transform;
	RectPoint* m_Rect;

private:
	DirectX::SimpleMath::Vector2 Screen_Resize;
	DirectX::SimpleMath::Vector2 Screen_Origin;
	DirectX::SimpleMath::Vector2 Screen_Ratio;

	float m_Left_Offset;
	float m_Right_Offset;
	float m_Top_Offset;
	float m_Bottom_Offset;

private:
	Eater::Delegate<> StartFunction;
	
	bool m_Start;

	UINT m_Layer;

	PIVOT_TYPE m_Pivot;
	DirectX::SimpleMath::Vector2 m_Pos;
	DirectX::SimpleMath::Vector3 m_Rot;
	DirectX::SimpleMath::Vector2 m_Scl;

	std::string m_TextureName;
	std::vector<std::string> m_TextureListName;
};

