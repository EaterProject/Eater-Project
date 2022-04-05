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

	void OnTriggerEnter(GameObject* Obj);
private:
	Transform*	mTransform;
	Vector3		Direction;
	int MonsterTag;
	const float BulletSpeed = 10;
};

