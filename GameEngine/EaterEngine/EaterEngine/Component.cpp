#include "Component.h"
#include "TimeManager.h"
#include "KeyinputManager.h"
#include "GameObject.h"

TimeManager* Component::mTimeManager = nullptr;
KeyinputManager* Component::mKeyInputManger = nullptr;

//�Լ��������� �ؽ��ڵ��
Component::Component()
{
	gameobject = nullptr;
	ComponentType = 0;
	OrderCount = FUNCTION_ORDER_CENTER;
}

Component::~Component()
{
	gameobject = nullptr;
}

void Component::SetManager(TimeManager* time, KeyinputManager* key)
{
	mTimeManager = time;
	mKeyInputManger = key;
}