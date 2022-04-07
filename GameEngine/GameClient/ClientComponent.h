#pragma once
#include "Component.h"
#include "ClientObjectManager.h"
//Client ���� ����� ���۳�Ʈ���� ��Ŭ������ ��ӹ���

class GameObject;
class ClientObjectManager;

class ObjectFactory;
class ObjectManager;
class ClientComponent : public Component
{
public:
	ClientComponent();
	virtual ~ClientComponent();
public:
	void SetOBjManager(ClientObjectManager* OBJ_GM){mObjectGM = OBJ_GM;}
	int		GetType();
	bool	GetLife();
	void	SetLife(bool isLife);
public:
	virtual void ReSet() = 0;
protected:
	ClientObjectManager* mObjectGM;
	size_t ObjType;
	bool isLife;

private:
	friend ObjectFactory;
	friend ObjectManager;
};

