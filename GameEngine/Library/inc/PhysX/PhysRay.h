#pragma once
#include "PhysEngineDLL.h"
#include "SimpleMath.h"

using namespace DirectX;
using namespace SimpleMath;

//Ray�� �浹�� ��ü�� ����
struct PHYS_ENGINEDLL RaycastHit
{
	Vector3 HitPoint	= { 0,0,0 };		//�浹 ��ġ
	int HitCount		= 0;				//�浹�� ������Ʈ ����
	int FaceIndex		= 0;				//�浹�� ������Ʈ�� face�ε���
};

class PHYS_ENGINEDLL PhysRayCast
{
public:
	PhysRayCast();
	~PhysRayCast();

	//Ray�� ��� ������ ��ġ
	void SetStartOrigin(float& x,float& y,float& z);
	void SetStartOrigin(float&& x,float&& y,float&& z);

	//Ray�� ����
	void SetDirection(float& x, float& y, float& z);
	void SetDirection(float&& x, float&& y, float&& z);

	//Ray�� ����
	void SetMaxDistance(float& _MaxDistance);
	void SetMaxDistance(float&& _MaxDistance);


	Vector3 Origin;			//�������� ��ġ
	Vector3 Direction;		//��� ����

	RaycastHit Hit;			//�浹�� ��ü ����
	float MaxDistance;		//�ִ� �Ÿ�
};
