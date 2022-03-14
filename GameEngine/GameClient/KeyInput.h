#pragma once
#include "Component.h"
class Transform;
class Keyinput :public Component
{
public:
	Keyinput();
	virtual ~Keyinput();

	virtual void Start();
	virtual void Update();

	Transform* transform;
private:
	float mLastMousePosX;
	float mLastMousePosY;
	float speed = 10;
};