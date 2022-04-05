#pragma once
#include "Component.h"
#include "ClientObjectManager.h"
//Client 에서 사용할 컨퍼넌트들은 이클래스를 상속받음

class GameObject;
class ClientObjectManager;
class ClientComponent : public Component
{
public:
	ClientComponent();
	virtual ~ClientComponent();
public:
	void SetOBjManager(ClientObjectManager* OBJ_GM){mObjectGM = OBJ_GM;}

	bool isLife = false;
	virtual void ReSet() = 0;
protected:
	ClientObjectManager* mObjectGM;
};

