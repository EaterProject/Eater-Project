#pragma once
#include "Component.h"
class GameObject;
class MonsterBase : public Component
{
public:
	MonsterBase();
	virtual ~MonsterBase();
public:
	bool isLife = false;
	void Awake();
	void SetPlayer(GameObject* Obj);
	GameObject* GetPlayer();
protected:
	void Reset();//���� ������ ���¸� �ʱ�ȭ ��Ų��
	void Create(float HP, float AttackPower, float AttackTime);	//�ʱⰪ ����
	void ErrorCheck();
	void SetPosition(float x, float y, float z);

	enum class STATE
	{
		ATTACK,
		MOVE,
		DEAD,
		CREATE,
	};

	GameObject* mPlayer;
	float HP;				//ü��
	float AttackPower;		//���ݷ�
	float AttackDelayTime;	//���� �ֱ�
	STATE State;
private:
	//�������ʴ� �ʱⰪ
	float Start_HP;
	float Start_AttackPower;
	float Start_AttackDelayTime;
	STATE Start_State;
};

