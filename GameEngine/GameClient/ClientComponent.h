#pragma once
#include "Component.h"
#include "ClientObjectManager.h"
//Client ���� ����� ���۳�Ʈ���� ��Ŭ������ ��ӹ���

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

