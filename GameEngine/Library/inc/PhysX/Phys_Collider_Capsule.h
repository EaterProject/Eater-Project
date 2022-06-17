#pragma once
#include "Phys_Collider_Interface.h"
class Phys_Collider_Capsule :public Phys_Collider_Interface
{
public:
	Phys_Collider_Capsule();
	virtual ~Phys_Collider_Capsule();

	void Initialize() override;
	void Release() override;

	void SetSize(float& Radius,float& Helght);
	

	float Radius;
	float Helght;
};

