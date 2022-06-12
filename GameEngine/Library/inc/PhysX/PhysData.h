#pragma once
#include "PhysEngineDLL.h"
#include "SimpleMath.h"
#include <vector>

#include "PhysScene.h"
#include "PhysMaterial.h"
#include "PhysCollider.h"

#pragma warning(disable : 4251)

using namespace DirectX;
using namespace SimpleMath;

/*
Dinamic 객체
	힘에 의해서 움직일 수도 있고 충돌도 가능한 객체

Knematic 객체
	한 위치에서 다른 위치에서 이동은 가능하지만 힘에 의해서
	움직일 수 있는 객체는 아니다

	무한 질량을 가진 물체로 간주하며 moveGlobal 함수로 움직일수 있다
	일반적인 Dynamic 액터를 밀수도 있다 하지만 static 과 다른 Knematic 물체와 충돌하지않음
Static 객체
	이객체는 어떠한 방법을 쓰더라도 움직일 수 없는 객체이다
	고정된 물체를 의미한다
*/

class Phys_Collider_Capsule;
class Phys_Collider_Sphere;
class Phys_Collider_Triangle;
class Phys_Collider_Box;


///하나의 강체를 만드는 구조체 무슨내용인지 보고싶으면 아래쪽에 설명
///사용하지 않는 값은 그냥 디폴트값으로 놔두면된다
class  PHYS_ENGINEDLL PhysData
{
public:
	 PhysData();
	 ~PhysData();
	 void* EaterObj = nullptr;

	Vector3 WorldPosition;	//위치
	Vector4 Rotation;		//회전
	Vector3 CenterPoint;	//중심점
	//월드상의 위치
	 void SetWorldPosition(float x, float y, float z);
	 void SetLocalPosition(float x, float y, float z);

	//월드상의 회전
	 void SetRotation(float x, float y, float z);
	 void SetRotate(float x, float y, float z);
	 void AddForce(float x, float y, float z);
	 void SetVelocity(float x, float y, float z);
	 void SetTrigger(bool mTrigger);
public:
	 Vector3 GetVelocity();
	 bool GetTriggerEnter();
	 bool GetTriggerStay();
	 bool GetTriggerExit();
public:
	//무게
	float MT_Mass;
	PhysMaterial* mMeterial;
	PhysCollider* mCollider;
public:
	///객체 정보 데이터
	//움직이는 객체인지 (Dinamic , Static)
	bool isDinamic = true;
	//중력 작용 여부
	bool isGravity = true;
	//움직이진 않는데 충돌 할것인지
	bool isKinematic;

	int TriggerCount = 0;


	int Enter_Count		= 0;
	int Stay_Count		= 0;
	int Exit_Count		= 0;
	PhysData* TriggerEnter_List[10];
	PhysData* TriggerStay_List[10];
	PhysData* TriggerExit_List[10];
	//PhysData* TriggerList[10];
public:
	//이동에 관한 축변환을 막는다
	void SetLockAxis_Position(bool x, bool y, bool z);
	Vector3 GetLockAxis_Position();
	//회전에 관한 축변환을 막는다
	void SetLockAxis_Rotation(bool x, bool y, bool z);
	Vector3 GetLockAxis_Rotation();

private:
	///위치 데이터
	//로컬 위치 (계층 구조일때)
	Vector3 LocalPosition;
	//어떠한 축 이동을 고정 시킬때 사용 
	Vector3 FreezePositon;
	//어떠한 축 회전을 고정 시킬때 사용
	Vector3 FreezeRotaticon;
	//이객체의 이동속력을 넣어준다
	Vector3 Velocity;
	//PhysX쪽의 방향을 가져온다
	Vector3 PhysX_Velocity;
	//현재 벡터 방향으로 힘을 줄때 사용
	Vector3 Force;
private:
	bool isMove;
	bool isForce;
	bool isVelocity;
	bool isPosition;
	bool isTrigger; 

	bool OnTriggerEnter;
	bool OnTriggerStay;
	bool OnTriggerExit;
private:
	void* ActorObj = nullptr;
	
	void PushTriggerEnter_Data(PhysData* Data);
	void PushTriggerExit_Data(PhysData* Data);

	friend class Factory;
	friend class PhysEngine;
	friend class BaseEventCallBack;
};

struct FilterGroup
{
	enum Enum
	{
		eWall = (1 << 0),
		ePlayer = (1 << 0),
	};
};


