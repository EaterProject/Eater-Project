#pragma once
#include "ClientComponent.h"

class GameObject;
class Transform;
class AttackDrone : public ClientComponent
{
public:
	AttackDrone();
	~AttackDrone();

	void SetUp();
	void Update();
	void ReSet();
	void OnTriggerStay(GameObject* Obj);
private:
	int MonsterTag;
	Transform* mTransform;
	Transform* mMonsterTR;
	float AttackTime = 0;
	const float AttackMaxTime = 1.0f;
};

