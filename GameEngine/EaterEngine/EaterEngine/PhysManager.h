#pragma once
class PhysEngine;

#include "SimpleMath.h"
class PhysRayCast;
class PhysManager
{
public:
	PhysManager();
	~PhysManager();


	void Initialize();	//초기화 
	void Release();		//삭제
	void Update(float m_Time);

	static bool RayCast(PhysRayCast* ray);

private:
	PhysEngine* Phys;
};