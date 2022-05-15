#pragma once
#include "ClientComponent.h"
class Transform;
class Bullet : public ClientComponent
{
public:
	Bullet();
	~Bullet();

	void SetUp();
	void Update();
	void ReSet();

	void OnTriggerEnter(GameObject* Obj);

	void Shooting(Vector3 Dir);
private:
	Transform*	mTransform;
	Vector3		Direction;

	float LifeTime = 0;
	int MonsterTag;
	const float ResetTime	= 5.0f;
	const float BulletSpeed = 10.0f;
};

