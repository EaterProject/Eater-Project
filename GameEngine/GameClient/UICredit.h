#pragma once
#include <Component.h>

class Image;
class RectTransform;

class UICredit : public Component
{
public:
	UICredit();
	~UICredit();

public:
	void Awake() override;
	void Update() override;

public:
	void Set_CreditUI_Active(bool active);

private:
	bool StartCredit;

	float MoveSpeed;

	float StartWidth;
	float StartHeight;
	float EndHeight;
private:
	Image* CreditImage;
	RectTransform* CreditTransform;
};

