#pragma once
#include "Phys_Collider_Interface.h"
class Phys_Collider_Sphere :public Phys_Collider_Interface
{
public:
	Phys_Collider_Sphere();
	virtual ~Phys_Collider_Sphere();

	virtual void Initialize() override;
	virtual void Release() override;

	void SetSize(float Radius);

	float Radius = 0;
};

