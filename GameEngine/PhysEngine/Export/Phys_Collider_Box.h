#pragma once
#include "Phys_Collider_Interface.h"
class Phys_Collider_Box :public Phys_Collider_Interface
{
public:
	Phys_Collider_Box();
	virtual ~Phys_Collider_Box();

	PHYS_ENGINEDLL void Initialize() override;
	PHYS_ENGINEDLL void Release() override;

	PHYS_ENGINEDLL void SetSize(float& x, float& y, float& z);
	PHYS_ENGINEDLL Phys_Base_Vector3 GetSize();

public:
	Phys_Base_Vector3 Size;
};

