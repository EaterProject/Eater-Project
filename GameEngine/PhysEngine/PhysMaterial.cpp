#include "PhysMaterial.h"

PhysMaterial::PhysMaterial()
{
	float MT_StaticFriction = 0.6f;
	//운동 마찰력
	float MT_DynamicFriction = 0.6f;
	//복원력
	float MT_Restitution = 0;
}

PhysMaterial::~PhysMaterial()
{

}
