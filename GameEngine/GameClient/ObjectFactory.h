#pragma once

/// <summary>
/// ������Ʈ�� ������ �����ϴ� ������Ʈ ���丮
/// </summary>
#include <vector>
enum class MONSTER_TYPE
{
	MONSTER_A, //������� ���� 
	MONSTER_B, //�������� ����
};

class GameObject;
class MonsterBase;
class ObjectFactory
{
public:
	ObjectFactory();
	~ObjectFactory();

	void Initialize();
	void Release();


	GameObject* CreateCameraPlayer();												//�÷��̾� ī�޶� ��ü�� ����
	GameObject* CreatePlayer();														//�÷��̾� ��ü�� ����
	MonsterBase* CreateMonster(float x, float y, float z , MONSTER_TYPE Type);		//���� ��ü�� ����
	GameObject* CreateMana();														//������ ��ü�� ����
	GameObject* CreateDrone();														//��� ��ü�� ����
	GameObject* CreatePortal();														//���Ͱ�ü�� ������ ��Ż ����
private:
	std::vector<GameObject*> ObjectManager;

	//���ӻ� �Ѱ��� �����ϴ� ������Ʈ�� ���� ����
	GameObject* PlayerObject;
	GameObject* PlayerMainCamera;
};

