#include "Component.h"
#include "TimeManager.h"
#include "KeyinputManager.h"
#include "GameObject.h"

TimeManager* Component::mTimeManager = nullptr;
KeyinputManager* Component::mKeyInputManger = nullptr;

//함수포인터의 해쉬코드들
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