#pragma once
#include "GameObject.h"
#include <string>
//상태 패턴 인터페이스
class Transform;
class AnimationController;
class State
{
public:
	State() = default;
	virtual ~State() = default;
public:
	//상태를 실행시킬 게임 오브젝트를 넣어준다,그리고 필요한 컨포넌트를 가져오는 함수
	virtual void SetObject(GameObject* Object);
	virtual int Play() = 0;

	//각종 상태에 메세지를 주고싶을때(보스의 패턴을 변경, 공격을 맞았을떄 등등...)
	virtual void SetMessage(void* Message) {};

	//타겟 오브젝트
	void SetTarget(GameObject* mTarget);
	void SetTarget(Transform* mTarget);

	//애니메이션의 이름을 넣어준다
	void SetAnimationOption(std::string& Name,float Speed = 1.0f);
	void SetAnimationOption(std::string&& Name, float Speed = 1.0f);
protected:
	GameObject* mObject				= nullptr;			//현재 상태를 실행시킬 오브젝트
	AnimationController* mAnimation = nullptr;			//현재 오브젝트의 애니메이션
	Transform*	mTransform			= nullptr;			//현재 오브젝트의 위치
	Transform*	mTargetTR			= nullptr;			//타겟의 위치

	//상태에 애니메이션을 재생시켜야 하는 상황이 많이있기때문에
	std::string AnimationName	= "";				//현재 상태의 애니메이션 이름
	float AnimationSpeed		= 1.0f;				//현재 상태의 애니메이션 속도
};

