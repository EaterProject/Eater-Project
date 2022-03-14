#pragma once
#include "EaterEngineDLL.h"

class GameObject;
class IComponent
{
public:
	EATER_ENGINEDLL IComponent() = default;
	virtual ~IComponent() = default;

public:
	//활성화 여부
	bool Enabled = true;

protected:
	int OrderCount = FUNCTION_ORDER_CENTER;

	//이컨퍼넌트를 가지고 있는 게임오브젝트
	GameObject* gameobject;

protected:
	///어떤컨퍼넌트의 함수가 함수리스트에 몇번째로 실행될것인지 여부
	//ex)MeshFilter의 StartUpdate가 Transform의 StartUpdate 보다 먼저 실행되어야 한다면
	//MeshFileter 의 TransformUpdate_Order = 0; 가장첫번쨰
	//Transform 의 TransformUpdate_Order = 4; 가장 마지막으로 넣는다
	//0 = 제일 먼저 , 4 = 제일 나중에
	const int FUNCTION_ORDER_FIRST = 0;
	const int FUNCTION_ORDER_CENTER = 2;
	const int FUNCTION_ORDER_LAST = 4;

	unsigned int Awake_Order = FUNCTION_ORDER_CENTER;
	unsigned int Start_Order = FUNCTION_ORDER_CENTER;
	unsigned int StartUpdate_Order = FUNCTION_ORDER_CENTER;
	unsigned int TransformUpdate_Order = FUNCTION_ORDER_CENTER;
	unsigned int PhysicsUpdate_Order = FUNCTION_ORDER_CENTER;
	unsigned int DefaultUpdate_Order = FUNCTION_ORDER_CENTER;
	unsigned int EndUpdate_Order = FUNCTION_ORDER_CENTER;
private:
	//게임 오브젝트 안에 컨퍼넌트를 찾을때 이값으로 찾아옴
	size_t ComponentType;

	//이컨퍼넌트의 어떤함수가 오버라이딩되어있는지 확인하기위해
	unsigned int FUNCTION_MASK = 0x00000000;


protected:
	//시작 단계에 가장먼저 실행되는 함수
	virtual void Awake() {};
	//시작 함수
	virtual void Start() {};
	//처음
	virtual void StartUpdate() {};
	//이동 행렬 업데이트
	virtual void TransformUpdate() {};
	//물리 업데이트
	virtual void PhysicsUpdate() {};
	//일반 업데이트
	virtual void Update() {};
	//마지막 업데이트
	virtual void EndUpdate() {};
};
