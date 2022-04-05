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
class MonsterA;
class MonsterB;
class AttackDrone;
class HealingDrone;
class Bullet;
class ManaStone;
class ClientObjectManager;
class Potal;


class ObjectFactory
{
public:
	ObjectFactory();
	~ObjectFactory();
public:
	void Initialize(ClientObjectManager* mGM);
	void Release();

private:
	GameObject*				CreatePlayer();									//�÷��̾� ��ü�� ����
	Bullet*					CreateBullet(float x,float y,float z);			//�Ѿ� ��ü�� ����
	MonsterA*				CreateMonsterA(float x, float y, float z);		//���� ��ü�� ����
	MonsterB*				CreateMonsterB(float x, float y, float z);		//���� ��ü�� ����
	ManaStone*				CreateManaStone();								//������ ��ü�� ����
	HealingDrone*			CreateHealingDrone();							//���� ��� ��ü�� ����
	AttackDrone*			CreateAttackDrone(float x, float y, float z);	//���� ��� ��ü�� ����
	Potal*					CreatePortal();									//���Ͱ�ü�� ������ ��Ż ����
private:
	//���ӻ� �Ѱ��� �����ϴ� ������Ʈ�� ���� ����
	GameObject* PlayerObject;
	GameObject* PlayerMainCamera;

	ClientObjectManager* mOBJ_GM;
};

