#include "Bullet.h"
#include "Transform.h"
#include "GameObject.h"
#include "MainHeader.h"

Bullet::Bullet()
{
	mTransform = nullptr;
	isLife = false;
	Direction = { 0,0,0 };
}

Bullet::~Bullet()
{
	mTransform = nullptr;
	isLife = false;
	Direction = { 0,0,0 };
}

void Bullet::SetUp()
{
	mTransform = gameobject->GetTransform();
}

void Bullet::Shooting(Vector3 Dir)
{
	Direction = Dir;
}

void Bullet::Update()
{
	if (isLife == true)
	{
		mTransform->SetTranlate(Direction * GetDeltaTime());
	}
}

void Bullet::ReSet()
{
	isLife = false;
}
