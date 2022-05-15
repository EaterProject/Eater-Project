#include "ClientComponent.h"
#include <typeinfo>

ClientComponent::ClientComponent()
{
	isLife		= false;
	mObjectGM	= nullptr;
	ObjType		= 0;
}

ClientComponent::~ClientComponent()
{
	isLife = false;
	mObjectGM = nullptr;
	ObjType = 0;
}

int ClientComponent::GetType()
{
	return (int)typeid(this).hash_code();
}

bool ClientComponent::GetLife()
{
	return isLife;
}

void ClientComponent::SetLife(bool Life)
{
	isLife = Life;
}
