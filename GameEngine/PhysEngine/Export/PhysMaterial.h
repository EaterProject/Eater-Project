#pragma once
#include "PhysEngineDLL.h"


///콜라이더의 재질
/*
 복원력 (Restitution)
	당구공을 예로 들자면 움직이는 공이 가만히있는 공과 충돌했을떄 움직이는 공은 멈추고
	멈춰있던 공은 움직이게 된다 이때 작용되는것이 복원력 1일수록 충돌하였을떄 속도가 보존된다
	0이라면 충돌 하자마자 멈춘다 즉 1 = 완전 탄성  0 = 비탄성 충돌

 정지 마찰력 (StaticFriction),운동 마찰력 (DynamicFriction)
	물체가 움직이기 위한 마찰력을 의미 예를 들자면 가만히 있는 자동차를 처음움직이기는 힘들지만
	움직이기 시작하면 큰힘이 들지않는다 물체가 움직이기 시작하면 운동 마찰력이 적용된다 많은 경우에
	운동 마찰력은 정지 마찰력보다 작기때문에

	F(힘) = u(마찰계수) * m(질량) * g(중력)
*/
class PHYS_ENGINEDLL PhysMaterial
{
public:
	PhysMaterial();
	~PhysMaterial();
public:
	float MT_StaticFriction		= 0.5f;
	float MT_DynamicFriction	= 0.5f;
	float MT_Restitution		= 0.6f;
};

