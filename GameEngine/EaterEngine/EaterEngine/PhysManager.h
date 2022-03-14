#pragma once
class PhysEngine;

#include "SimpleMath.h"
class PhysRayCast;
class PhysManager
{
public:
	PhysManager();
	~PhysManager();


	void Initialize();	//�ʱ�ȭ 
	void Release();		//����
	void Update(float m_Time);

	static bool RayCast(PhysRayCast* ray);

private:
	PhysEngine* Phys;
};