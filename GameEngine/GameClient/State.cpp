#include "State.h"

#include "GameObject.h"
#include "Transform.h"
#include "AnimationController.h"
void State::SetObject(GameObject* Object)
{
	mObject = Object;
	mAnimation = mObject->GetComponent<AnimationController>();
	mTransform = mObject->GetTransform();
}
void State::SetTarget(GameObject* mTarget)
{
	mTargetTR = mTarget->GetTransform();
}
void State::SetTarget(Transform* mTarget)
{
	mTargetTR = mTarget;
}
void State::SetAnimationOption(std::string& Name, float Speed)
{
	AnimationName = Name;
	AnimationSpeed = Speed;
}

void State::SetAnimationOption(std::string&& Name, float Speed)
{
	AnimationName = Name;
	AnimationSpeed = Speed;
}
