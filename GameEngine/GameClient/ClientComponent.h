#pragma once
#include "Component.h"
#include "MessageManager.h"
//Client ���� ����� ���۳�Ʈ���� ��Ŭ������ ��ӹ���

class GameObject;
class MessageManager;

class ObjectFactory;
class ObjectManager;
class ClientComponent : public Component
{
public:
	ClientComponent();
	virtual ~ClientComponent();
public:
	void	SetOBjManager(MessageManager* OBJ_GM){mObjectGM = OBJ_GM;}
	int		GetType();
	bool	GetLife();
	void	SetLife(bool isLife);
public:
protected:
	MessageManager* mObjectGM;
	size_t ObjType;
	bool isLife;

private:
	friend ObjectFactory;
	friend ObjectManager;
};

