#include "PhysRay.h"
PhysRayCast::PhysRayCast()
{


}

PhysRayCast::~PhysRayCast()
{


}

void PhysRayCast::SetStartOrigin(float& x, float& y, float& z)
{
	Origin = { x,y,z };
}

void PhysRayCast::SetStartOrigin(float&& x, float&& y, float&& z)
{
	Origin = { x,y,z };
}

void PhysRayCast::SetDirection(float& x, float& y, float& z)
{
	Direction = { x,y,z };
}

void PhysRayCast::SetDirection(float&& x, float&& y, float&& z)
{
	Direction = { x,y,z };
}

void PhysRayCast::SetMaxDistance(float& _MaxDistance)
{
	MaxDistance = _MaxDistance;
}

void PhysRayCast::SetMaxDistance(float&& _MaxDistance)
{
	MaxDistance = _MaxDistance;
}