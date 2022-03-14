#pragma once
/// <summary>
/// 컨퍼넌트들의 최상위 클래스
/// 이클래스를 상속하고 만든 컨퍼넌트들은
/// 나중에 게임오브젝트를 만들고 AddComponent를 할떄 오버라이딩을 했는지 안했는지 검사 후
/// 업데이트 함수 포인터 리스트에 넣어줌
/// </summary>

#include "EaterEngineDLL.h"

class TimeManager;
class KeyinputManager;
class GameObject;

class Component
{
public:
	EATER_ENGINEDLL Component();
	virtual EATER_ENGINEDLL ~Component();

	//활성화 여부
	bool Enabled	= true;
public:
	//게임 오브젝트 설정
	virtual void SetObjectData() {}

	//매니저 넣어주기
	static void SetManager(TimeManager* time, KeyinputManager* key);

	//이컨퍼넌트를 가지고 있는 게임오브젝트
	GameObject* gameobject;
protected:
	int OrderCount  = FUNCTION_ORDER_CENTER;

	//Dll안에서만 사용될 매니저들
	static TimeManager*		mTimeManager;
	static KeyinputManager*	mKeyInputManger;
protected:
	///초기화 단계
	//시작 단계 필요한 컨퍼넌트 Get
	virtual void Awake() {};
	//컨퍼넌트 Initialize 
	virtual void SetUp() {};
	//초기화된값으로 컨퍼넌트 실행
	virtual void Start() {};

	///업데이트 단계
	//시작 단계 업데이트
	virtual void StartUpdate() {};
	//이동 행렬 업데이트
	virtual void TransformUpdate() {};
	//물리 업데이트
	virtual void PhysicsUpdate() {};
	//일반 업데이트
	virtual void Update() {};
	//마지막 업데이트
	virtual void EndUpdate() {};

protected:
	///어떤컨퍼넌트의 함수가 함수리스트에 몇번째로 실행될것인지 여부
	//ex)MeshFilter의 StartUpdate가 Transform의 StartUpdate 보다 먼저 실행되어야 한다면
	//MeshFileter 의 TransformUpdate_Order = 0; 가장첫번쨰
	//Transform 의 TransformUpdate_Order = 4; 가장 마지막으로 넣는다
	//0 = 제일 먼저 , 4 = 제일 나중에
	const int FUNCTION_ORDER_FIRST		= 0;
	const int FUNCTION_ORDER_CENTER		= 2;
	const int FUNCTION_ORDER_LAST		= 4;
	
	unsigned int Awake_Order			= FUNCTION_ORDER_CENTER;
	unsigned int Start_Order			= FUNCTION_ORDER_CENTER;
	unsigned int SetUp_Order			= FUNCTION_ORDER_CENTER;


	unsigned int StartUpdate_Order		= FUNCTION_ORDER_CENTER;
	unsigned int TransformUpdate_Order	= FUNCTION_ORDER_CENTER;
	unsigned int PhysicsUpdate_Order	= FUNCTION_ORDER_CENTER;
	unsigned int DefaultUpdate_Order	= FUNCTION_ORDER_CENTER;
	unsigned int EndUpdate_Order		= FUNCTION_ORDER_CENTER;
private:
	//게임 오브젝트 안에 컨퍼넌트를 찾을때 이값으로 찾아옴
	size_t ComponentType;

	//이컨퍼넌트의 어떤함수가 오버라이딩되어있는지 확인하기위해
	unsigned int FUNCTION_MASK = 0x00000000;

	
	//접근 여부를 아래 클래스들에게는 열어둠
	friend class GameObject;
	friend class ObjectManager;
};



