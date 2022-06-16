#include "Phys_Collider_Box.h"

Phys_Collider_Box::Phys_Collider_Box()
{
}

Phys_Collider_Box::~Phys_Collider_Box()
{

}

void Phys_Collider_Box::Initialize()
{

}

void Phys_Collider_Box::Release()
{

}

void Phys_Collider_Box::SetSize(float& x, float& y, float& z)
{
	Size.x = x;
	Size.y = y;
	Size.z = z;
}

Phys_Base_Vector3 Phys_Collider_Box::GetSize()
{
	return Size;
}
