#pragma once
#include "PhysEngineDLL.h"
#include "SimpleMath.h"

using namespace DirectX;
using namespace SimpleMath;

//Ray와 충돌한 객체의 정보
struct PHYS_ENGINEDLL RaycastHit
{
	Vector3 HitPoint	= { 0,0,0 };		//충돌 위치
	int HitCount		= 0;				//충돌한 오브젝트 개수
	int FaceIndex		= 0;				//충돌한 오브젝트의 face인덱슨
};

class PHYS_ENGINEDLL PhysRayCast
{
public:
	PhysRayCast();
	~PhysRayCast();

	//Ray의 쏘기 시작한 위치
	void SetStartOrigin(float& x,float& y,float& z);
	void SetStartOrigin(float&& x,float&& y,float&& z);

	//Ray의 방향
	void SetDirection(float& x, float& y, float& z);
	void SetDirection(float&& x, float&& y, float&& z);

	//Ray의 길이
	void SetMaxDistance(float& _MaxDistance);
	void SetMaxDistance(float&& _MaxDistance);


	Vector3 Origin;			//쏘기시작한 위치
	Vector3 Direction;		//쏘는 방향

	RaycastHit Hit;			//충돌한 객체 정보
	float MaxDistance;		//최대 거리
};
