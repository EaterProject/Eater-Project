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

class ClientComponent;
class GameObject;
class MonsterA;
class MonsterB;
class AttackDrone;
class HealingDrone;
class Bullet;
class ManaStone;
class MessageManager;
class Potal;
class FontImage;
class ComboFont;
class UICanvas;

class ObjectFactory
{
public:
	ObjectFactory();
	~ObjectFactory();
public:
	void Release();
		
	GameObject* CreatePlayer();			//�÷��̾� ��ü�� ����
	GameObject* CreateBullet();			//�Ѿ� ��ü�� ����
	GameObject* CreateMonsterA();		//���� ��ü�� ����
	GameObject* CreateMonsterB();		//���� ��ü�� ����
	GameObject* CreateManaStone();		//������ ��ü�� ����
	GameObject* CreateFontImage();		//��Ʈ �̹��� ����
	GameObject* CreateUICanvas();	//�÷��̾� ������Ʈ UI����
	GameObject* CreateGate_In();
	GameObject* CreateGate_Out();
	GameObject* CreateGate_Manager();
private:
	//���ӻ� �Ѱ��� �����ϴ� ������Ʈ�� ���� ����
	GameObject* PlayerObject;
	GameObject* PlayerMainCamera;

	std::vector<GameObject*> ManaPoint_List;
	std::vector<MonsterA*> MonsterA_List;
	std::vector<MonsterB*> MonsterB_List;
	ComboFont* mComboFont;

	friend MessageManager;
};

