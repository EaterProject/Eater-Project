#pragma once
#include "Component.h"
#include "Utility.h"

class Image;
class RectTransform;
class Button : public Component
{
public:
	EATER_ENGINEDLL Button();
	virtual ~Button();

public:
	void Awake() override;
	void Start() override;
	void Update() override;

public:
	EATER_ENGINEDLL void PushOnClickEvent(std::function<void()>& eventFunc);
	EATER_ENGINEDLL void PopOnClickEvent(std::function<void()>& eventFunc);
	EATER_ENGINEDLL void ResetOnClickEvent();

private:
	Eater::Delegate<> m_OnClickEvent;

	Image* m_Image;
	RectTransform* m_Transform;
};

