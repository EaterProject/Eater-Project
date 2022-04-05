#pragma once
#include "ClientComponent.h"
class Transform;
class Bullet : public ClientComponent
{
public:
	Bullet();
	~Bullet();

	void SetUp();
	void Shooting(Vector3 Dir);
	void Update();
	void ReSet();
private:
	Transform*	mTransform;
	Vector3		Direction;
};

