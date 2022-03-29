#pragma once

/// <summary>
/// ������Ʈ�� ������ �����ϴ� ������Ʈ ���丮
/// </summary>
#include <vector>

class GameObject;
class ObjectFactory
{
public:
	ObjectFactory();
	~ObjectFactory();

	void Initialize();
	void Release();

	GameObject* CreateCameraPlayer();	//�÷��̾� ī�޶� ��ü�� ����
	GameObject* CreatePlayer();			//�÷��̾� ��ü�� ����
	GameObject* CreateMonster(float x, float y, float z);		//���� ��ü�� ����
	GameObject* CreateMana();			//������ ��ü�� ����
	GameObject* CreateDrone();			//��� ��ü�� ����
	GameObject* CreatePortal();			//���Ͱ�ü�� ������ ��Ż ����
private:
	std::vector<GameObject*> ObjectManager;

	//���ӻ� �Ѱ��� �����ϴ� ������Ʈ�� ���� ����
	GameObject* PlayerObject;
	GameObject* PlayerMainCamera;
};

