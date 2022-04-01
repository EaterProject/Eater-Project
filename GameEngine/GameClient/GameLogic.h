#pragma once

/// <summary>
/// Ŭ���̾�Ʈ �� ��� �Ŵ����� �����ϴ� ���� �Ŵ���
/// </summary>
#include <vector>

class GameObject;
class ObjectFactory;
class MonsterBase;
class MonsterManager;

class GameLogic
{
public:
	GameLogic();
	~GameLogic();

public:
	void Initialize();
	void Release();
	void Update();
private:
	///���� ���� �Լ���
	void SetCreateMonsterMemorySize(int CreateCount);						//���� �̸� ����
	MonsterBase* GetLifeMonter();											//�ൿ�ϰ����� ���� ���͸� �Ѱ� �����´�
	void CreateMonster(float CreateMaxTime,GameObject* CreatePointObject);	//�ش� �ð��� �Ѿ������ ������Ʈ��ġ�� ������ġ�� ���͸� ����
private:
	const float CreateMonsterMaxTime = 5.0f;	//���� ���� �ð� max
private:
	//������Ʈ ���� Ŭ����
	ObjectFactory*	FactoryGM; 
	MonsterManager* MonsterGM;
private:



	std::vector<GameObject*>	PotalList;
	std::vector<MonsterBase*>	MonsterList;
};

