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
class ClientObjectManager;
class Potal;
class FontImage;

class ObjectFactory
{
public:
	ObjectFactory();
	~ObjectFactory();
public:
	void Initialize(ClientObjectManager* mGM);
	void Release();
		
	static GameObject*		CreatePlayer();									//�÷��̾� ��ü�� ����
	static Bullet*			CreateBullet(float x, float y, float z);		//�Ѿ� ��ü�� ����
	static MonsterA*		CreateMonsterA(float x, float y, float z);		//���� ��ü�� ����
	static MonsterB*		CreateMonsterB(float x, float y, float z);		//���� ��ü�� ����
	static ManaStone*		CreateManaStone(float x, float y, float z);		//������ ��ü�� ����

	static FontImage*		CreateFontImage(float x, float y);
private:
	//���ӻ� �Ѱ��� �����ϴ� ������Ʈ�� ���� ����
	GameObject* PlayerObject;
	GameObject* PlayerMainCamera;
	ClientObjectManager* mOBJ_GM;
};

