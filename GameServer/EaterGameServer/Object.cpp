#include "Object.h"
#include "PhysX_API.h"

Object::Object()
{
	
}

Object::~Object()
{

}

void Object::Update()
{

}

void Object::Create()
{
	PhysX_Create_Actor(m_PhysData);
}
