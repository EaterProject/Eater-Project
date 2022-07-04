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
	GameObject* CreateMonsterA();		//���� ��ü�� ����
	GameObject* CreateMonsterB();		//���� ��ü�� ����
	GameObject* CreateManaStone();		//������ ��ü�� ����
	GameObject* CreateFontImage();		//��Ʈ �̹��� ����

	GameObject* CreateUICanvas();		//�÷��̾� ������Ʈ UI����
	GameObject* CreateUIEffect();		//����Ʈ UI
	GameObject* CreateUITitle();		//Ÿ��Ʋ UI
	GameObject* CreateUIStore();		//���� UI
	GameObject* CreateUIOption();		//�ɼ� UI
	GameObject* CreateUIPause();		//�Ͻ����� UI
	GameObject* CreateUIManual();		//�޴��� UI
	GameObject* CreateUICredit();		//ũ���� UI
	GameObject* CreateUIBoss();
	GameObject* CreateSceneEffect();

	GameObject* CreateGate_In();		//����Ʈ ��ü ����
	GameObject* CreateGate_Out();		//����Ʈ ��ü ����
	GameObject* CreateGate_Manager();	//����Ʈ �Ŵ��� ����
	GameObject* CreateCameraManager();	//ī�޶� �Ŵ��� ����

	GameObject* CreateBoss();			//���� ��ü ����
	GameObject* CreateBossWeapon();		//���� ���� ����
	GameObject* CreateBossFriend();		//���� �н� ����

	GameObject* CreateStore();
	GameObject* CreatePotal();
private:
	//���ӻ� �Ѱ��� �����ϴ� ������Ʈ�� ���� ����
	std::vector<GameObject*> ManaPoint_List;
	friend MessageManager;
};

