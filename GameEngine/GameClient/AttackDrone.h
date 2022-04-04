#pragma once

#include "Component.h"
class GameObject;
class Transform;
class AttackDrone : public Component
{
public:
	AttackDrone();
	~AttackDrone();

	void SetUp();
	void Update();
	void OnTriggerStay(GameObject* Obj);
private:
	int MonsterTag;
	Transform* mTransform;
	Transform* mMonsterTR;
};

