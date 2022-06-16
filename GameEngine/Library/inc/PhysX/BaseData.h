#pragma once
struct Phys_Base_Vector3
{
	float x = 0;
	float y = 0;
	float z = 0;

	void operator=(const Phys_Base_Vector3& v3)
	{
		this->x = v3.x;
		this->y = v3.y;
		this->z = v3.z;
	}
};